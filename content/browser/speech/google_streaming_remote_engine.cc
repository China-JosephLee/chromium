// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/speech/google_streaming_remote_engine.h"

#include <vector>

#include "base/bind.h"
#include "base/command_line.h"
#include "base/rand_util.h"
#include "base/string_number_conversions.h"
#include "base/string_util.h"
#include "base/time.h"
#include "base/utf_string_conversions.h"
#include "content/browser/speech/audio_buffer.h"
#include "content/browser/speech/proto/google_streaming_api.pb.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/common/content_switches.h"
#include "content/public/common/speech_recognition_error.h"
#include "content/public/common/speech_recognition_result.h"
#include "net/base/escape.h"
#include "net/base/load_flags.h"
#include "net/url_request/url_fetcher.h"
#include "net/url_request/url_request_context.h"
#include "net/url_request/url_request_context_getter.h"
#include "net/url_request/url_request_status.h"

using content::BrowserThread;
using content::SpeechRecognitionError;
using content::SpeechRecognitionErrorCode;
using content::SpeechRecognitionHypothesis;
using content::SpeechRecognitionResult;
using net::URLFetcher;

namespace {

// TODO(primiano): This shouldn't be a const, rather it should be taken from
// maxNBest property (which is not yet implemented in WebKit).
const int kMaxResults = 5;
const char kDownstreamUrl[] = "/down?";
const char kUpstreamUrl[] = "/up?";
const int kAudioPacketIntervalMs = 100;
const speech::AudioEncoder::Codec kDefaultAudioCodec =
    speech::AudioEncoder::CODEC_FLAC;

// TODO(primiano): /////////// Remove this after debug stage. /////////////////
void DumpResponse(const std::string& response) {
  bool parse_ok;
  speech::HttpStreamingResult res;
  parse_ok = res.ParseFromString(response);
  DVLOG(1) << "------------";
  if(!parse_ok) {
    DVLOG(1) << "Parse failed!";
    return;
  }
  if (res.has_id())
    DVLOG(1) << "ID\t" << res.id();
  if (res.has_status())
    DVLOG(1) << "STATUS\t" << res.status();
  if (res.has_upstream_connected())
    DVLOG(1) << "UPCON\t" << res.upstream_connected();
  if (res.hypotheses_size() > 0)
    DVLOG(1) << "HYPS\t" << res.hypotheses_size();
  if (res.has_provisional())
    DVLOG(1) << "PROV\t" << res.provisional();
  if (res.has_ephemeral())
    DVLOG(1) << "EPHM\t" << res.ephemeral();
  DVLOG(1) << "------------\n";
}

std::string GetWebserviceBaseURL() {
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  return command_line.GetSwitchValueASCII(
      switches::kSpeechRecognitionWebserviceURL);
}

std::string GetWebserviceKey() {
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  return command_line.GetSwitchValueASCII(
      switches::kSpeechRecognitionWebserviceKey);
}

}  // namespace

namespace speech {

const int GoogleStreamingRemoteEngine::kUpstreamUrlFetcherIdForTests = 0;
const int GoogleStreamingRemoteEngine::kDownstreamUrlFetcherIdForTests = 1;
const int GoogleStreamingRemoteEngine::kWebserviceStatusNoError = 0;
const int GoogleStreamingRemoteEngine::kWebserviceStatusErrorNoMatch = 5;

GoogleStreamingRemoteEngine::GoogleStreamingRemoteEngine(
    net::URLRequestContextGetter* context)
    : url_context_(context),
      encoder_(NULL),
      previous_response_length_(0),
      got_last_definitive_result_(false),
      is_dispatching_event_(false),
      state_(STATE_IDLE) {
}

GoogleStreamingRemoteEngine::~GoogleStreamingRemoteEngine() {}

void GoogleStreamingRemoteEngine::SetConfig(
    const SpeechRecognitionEngineConfig& config) {
  config_ = config;
}

void GoogleStreamingRemoteEngine::StartRecognition() {
  FSMEventArgs event_args(EVENT_START_RECOGNITION);
  DispatchEvent(event_args);
}

void GoogleStreamingRemoteEngine::EndRecognition() {
  FSMEventArgs event_args(EVENT_END_RECOGNITION);
  DispatchEvent(event_args);
}

void GoogleStreamingRemoteEngine::TakeAudioChunk(const AudioChunk& data) {
  FSMEventArgs event_args(EVENT_AUDIO_CHUNK);
  event_args.audio_data = &data;
  DispatchEvent(event_args);
}

void GoogleStreamingRemoteEngine::AudioChunksEnded() {
  FSMEventArgs event_args(EVENT_AUDIO_CHUNKS_ENDED);
  DispatchEvent(event_args);
}

void GoogleStreamingRemoteEngine::OnURLFetchComplete(const URLFetcher* source) {
  const bool kResponseComplete = true;
  DispatchHTTPResponse(source, kResponseComplete);
}

void GoogleStreamingRemoteEngine::OnURLFetchDownloadProgress(
    const URLFetcher* source, int64 current, int64 total) {
  const bool kPartialResponse = false;
  DispatchHTTPResponse(source, kPartialResponse);
}

void GoogleStreamingRemoteEngine::DispatchHTTPResponse(const URLFetcher* source,
                                                       bool end_of_response) {
  DCHECK(CalledOnValidThread());
  DCHECK(source);
  const bool response_is_good = source->GetStatus().is_success() &&
                                source->GetResponseCode() == 200;
  std::string response;
  if (response_is_good)
    source->GetResponseAsString(&response);
  const size_t current_response_length = response.size();

  // TODO(primiano): /////////// Remove this after debug stage. ////////////////
  DVLOG(1) << (source == downstream_fetcher_.get() ? "Downstream" : "Upstream")
           << "HTTP, code: " << source->GetResponseCode()
           << "      length: " << current_response_length
           << "      eor: " << end_of_response;

  // URLFetcher provides always the entire response buffer, but we are only
  // interested in the fresh data introduced by the last chunk. Therefore, we
  // drop the previous content we have already processed.
  if (current_response_length != 0) {
    DCHECK_GE(current_response_length, previous_response_length_);
    response.erase(0, previous_response_length_);
    previous_response_length_ = current_response_length;
  }

  if (!response_is_good && source == downstream_fetcher_.get()) {
    // TODO(primiano): /////////// Remove this after debug stage. /////////////
    DVLOG(1) << "Downstream error " << source->GetResponseCode();
    FSMEventArgs event_args(EVENT_DOWNSTREAM_ERROR);
    DispatchEvent(event_args);
    return;
  }
  if (!response_is_good && source == upstream_fetcher_.get()) {
    // TODO(primiano): /////////// Remove this after debug stage. /////////////
    DVLOG(1) << "Upstream error " << source->GetResponseCode()
             << " EOR " << end_of_response;
    FSMEventArgs event_args(EVENT_UPSTREAM_ERROR);
    DispatchEvent(event_args);
    return;
  }
  if (source == upstream_fetcher_.get())
    return;

  DCHECK(response_is_good && source == downstream_fetcher_.get());

  // The downstream response is organized in chunks, whose size is determined
  // by a 4 bytes prefix, transparently handled by the ChunkedByteBuffer class.
  // Such chunks are sent by the speech recognition webservice over the HTTP
  // downstream channel using HTTP chunked transfer (unrelated to our chunks).
  // This function is called every time an HTTP chunk is received by the
  // url fetcher. However there isn't any particular matching beween our
  // protocol chunks and HTTP chunks, in the sense that a single HTTP chunk can
  // contain a portion of one chunk or even more chunks together.
  chunked_byte_buffer_.Append(response);

  // A single HTTP chunk can contain more than one data chunk, thus the while.
  while (chunked_byte_buffer_.HasChunks()) {
    FSMEventArgs event_args(EVENT_DOWNSTREAM_RESPONSE);
    event_args.response = chunked_byte_buffer_.PopChunk();
    DCHECK(event_args.response.get());
    // TODO(primiano): /////////// Remove this after debug stage. /////////////
    DumpResponse(std::string((char*)&(*event_args.response->begin()),
                             event_args.response->size()));
    DispatchEvent(event_args);
  }
  if (end_of_response) {
    FSMEventArgs event_args(EVENT_DOWNSTREAM_CLOSED);
    DispatchEvent(event_args);
  }
}

bool GoogleStreamingRemoteEngine::IsRecognitionPending() const {
  DCHECK(CalledOnValidThread());
  return state_ != STATE_IDLE;
}

int GoogleStreamingRemoteEngine::GetDesiredAudioChunkDurationMs() const {
  return kAudioPacketIntervalMs;
}

// -----------------------  Core FSM implementation ---------------------------

void GoogleStreamingRemoteEngine::DispatchEvent(
    const FSMEventArgs& event_args) {
  DCHECK(CalledOnValidThread());
  DCHECK_LE(event_args.event, EVENT_MAX_VALUE);
  DCHECK_LE(state_, STATE_MAX_VALUE);

  // Event dispatching must be sequential, otherwise it will break all the rules
  // and the assumptions of the finite state automata model.
  DCHECK(!is_dispatching_event_);
  is_dispatching_event_ = true;

  // TODO(primiano): /////////// Remove this after debug stage. ////////////////
  //DVLOG(1) << "State " << state_ << ", Event " << event_args.event;
  state_ = ExecuteTransitionAndGetNextState(event_args);

  is_dispatching_event_ = false;
}

GoogleStreamingRemoteEngine::FSMState
GoogleStreamingRemoteEngine::ExecuteTransitionAndGetNextState(
    const FSMEventArgs& event_args) {
  const FSMEvent event = event_args.event;
  switch (state_) {
    case STATE_IDLE:
      switch (event) {
        case EVENT_START_RECOGNITION:
          return ConnectBothStreams(event_args);
        case EVENT_END_RECOGNITION:
        // Note AUDIO_CHUNK and AUDIO_END events can remain enqueued in case of
        // abort, so we just silently drop them here.
        case EVENT_AUDIO_CHUNK:
        case EVENT_AUDIO_CHUNKS_ENDED:
        // DOWNSTREAM_CLOSED can be received if we end up here due to an error.
        case EVENT_DOWNSTREAM_CLOSED:
          return DoNothing(event_args);
        case EVENT_UPSTREAM_ERROR:
        case EVENT_DOWNSTREAM_ERROR:
        case EVENT_DOWNSTREAM_RESPONSE:
          return NotFeasible(event_args);
      }
      break;
    case STATE_BOTH_STREAMS_CONNECTED:
      switch (event) {
        case EVENT_AUDIO_CHUNK:
          return TransmitAudioUpstream(event_args);
        case EVENT_DOWNSTREAM_RESPONSE:
          return ProcessDownstreamResponse(event_args);
        case EVENT_AUDIO_CHUNKS_ENDED:
          return CloseUpstreamAndWaitForResults(event_args);
        case EVENT_END_RECOGNITION:
          return AbortSilently(event_args);
        case EVENT_UPSTREAM_ERROR:
        case EVENT_DOWNSTREAM_ERROR:
        case EVENT_DOWNSTREAM_CLOSED:
          return AbortWithError(event_args);
        case EVENT_START_RECOGNITION:
          return NotFeasible(event_args);
      }
      break;
    case STATE_WAITING_DOWNSTREAM_RESULTS:
      switch (event) {
        case EVENT_DOWNSTREAM_RESPONSE:
          return ProcessDownstreamResponse(event_args);
        case EVENT_DOWNSTREAM_CLOSED:
          return RaiseNoMatchErrorIfGotNoResults(event_args);
        case EVENT_END_RECOGNITION:
          return AbortSilently(event_args);
        case EVENT_UPSTREAM_ERROR:
        case EVENT_DOWNSTREAM_ERROR:
          return AbortWithError(event_args);
        case EVENT_START_RECOGNITION:
        case EVENT_AUDIO_CHUNK:
        case EVENT_AUDIO_CHUNKS_ENDED:
          return NotFeasible(event_args);
      }
      break;
  }
  return NotFeasible(event_args);
}

// ----------- Contract for all the FSM evolution functions below -------------
//  - Are guaranteed to be executed in the same thread (IO, except for tests);
//  - Are guaranteed to be not reentrant (themselves and each other);
//  - event_args members are guaranteed to be stable during the call;

GoogleStreamingRemoteEngine::FSMState
GoogleStreamingRemoteEngine::ConnectBothStreams(const FSMEventArgs&) {
  DCHECK(!upstream_fetcher_.get());
  DCHECK(!downstream_fetcher_.get());

  encoder_.reset(AudioEncoder::Create(kDefaultAudioCodec,
                                      config_.audio_sample_rate,
                                      config_.audio_num_bits_per_sample));
  DCHECK(encoder_.get());
  const std::string request_key = GenerateRequestKey();

  // Setup downstream fetcher.
  std::vector<std::string> downstream_args;
  downstream_args.push_back("sky=" + GetWebserviceKey());
  downstream_args.push_back("pair=" + request_key);
  downstream_args.push_back("maxresults=" + base::IntToString(kMaxResults));

  GURL downstream_url(GetWebserviceBaseURL() + std::string(kDownstreamUrl) +
                      JoinString(downstream_args, '&'));
  // TODO(primiano): /////////// Remove this after debug stage. /////////////
  DVLOG(1) << "Opening downstream: " + downstream_url.PathForRequest();

  downstream_fetcher_.reset(URLFetcher::Create(
      kDownstreamUrlFetcherIdForTests, downstream_url, URLFetcher::GET, this));
  downstream_fetcher_->SetRequestContext(url_context_);
  downstream_fetcher_->SetLoadFlags(net::LOAD_DO_NOT_SAVE_COOKIES |
                                    net::LOAD_DO_NOT_SEND_COOKIES |
                                    net::LOAD_DO_NOT_SEND_AUTH_DATA);
  downstream_fetcher_->Start();

  // Setup upstream fetcher.
  // TODO(primiano): Handle config_.grammar array when it will be implemented by
  // the speech recognition webservice.
  std::vector<std::string> upstream_args;
  upstream_args.push_back("sky=" + GetWebserviceKey());
  upstream_args.push_back("pair=" + request_key);
  upstream_args.push_back(
      "lang=" + net::EscapeQueryParamValue(GetAcceptedLanguages(), true));
  upstream_args.push_back(
      config_.filter_profanities ? "pfilter=2" : "pfilter=0");
  upstream_args.push_back("maxresults=" + base::IntToString(kMaxResults));
  upstream_args.push_back("client=myapp.mycompany.com");
  // TODO(primiano): Can we remove this feature sending audio HW information?
  if (!config_.hardware_info.empty()) {
    upstream_args.push_back(
        "xhw=" + net::EscapeQueryParamValue(config_.hardware_info, true));
  }

  GURL upstream_url(GetWebserviceBaseURL() + std::string(kUpstreamUrl) +
                    JoinString(upstream_args, '&'));

  // TODO(primiano): /////////// Remove this after debug stage. ////////////////
  DVLOG(1) << "Opening upstream: " + upstream_url.PathForRequest();

  upstream_fetcher_.reset(URLFetcher::Create(
      kUpstreamUrlFetcherIdForTests, upstream_url, URLFetcher::POST, this));
  upstream_fetcher_->SetChunkedUpload(encoder_->mime_type());
  upstream_fetcher_->SetRequestContext(url_context_);
  upstream_fetcher_->SetReferrer(config_.origin_url);
  upstream_fetcher_->SetLoadFlags(net::LOAD_DO_NOT_SAVE_COOKIES |
                                  net::LOAD_DO_NOT_SEND_COOKIES |
                                  net::LOAD_DO_NOT_SEND_AUTH_DATA);
  upstream_fetcher_->Start();
  previous_response_length_ = 0;
  return STATE_BOTH_STREAMS_CONNECTED;
}

GoogleStreamingRemoteEngine::FSMState
GoogleStreamingRemoteEngine::TransmitAudioUpstream(
    const FSMEventArgs& event_args) {
  DCHECK(upstream_fetcher_.get());
  DCHECK(event_args.audio_data.get());
  const AudioChunk& audio = *(event_args.audio_data);

  DCHECK_EQ(audio.bytes_per_sample(), config_.audio_num_bits_per_sample / 8);
  encoder_->Encode(audio);
  scoped_refptr<AudioChunk> encoded_data(encoder_->GetEncodedDataAndClear());
  upstream_fetcher_->AppendChunkToUpload(encoded_data->AsString(), false);
  return state_;
}

GoogleStreamingRemoteEngine::FSMState
GoogleStreamingRemoteEngine::ProcessDownstreamResponse(
    const FSMEventArgs& event_args) {
  DCHECK(event_args.response.get());
  bool is_definitive_result = false;

  HttpStreamingResult ws_result;
  const bool protobuf_parse_successful = ws_result.ParseFromArray(
      &(*event_args.response->begin()),
      event_args.response->size());
  if (!protobuf_parse_successful)
    return AbortWithError(event_args);

  // TODO(primiano): The code below sounds to me like a hack. Discuss the
  // possibility of having a simpler and clearer protobuf grammar, in order to
  // distinguish the different type of results and errors in a civilized way.

  // Skip the upstream connected notification, since we're not interested in it.
  if (ws_result.has_upstream_connected())
    return state_;

  if (ws_result.has_status()) {
    switch (ws_result.status()) {
      case kWebserviceStatusNoError:
        is_definitive_result = true;
        break;
      case kWebserviceStatusErrorNoMatch:
        // TODO(primiano): Contact gshires@, in case of no results, instead of
        // the expected kWebserviceStatusErrorNoMatch status code we receive a
        // provisional-like result with no provisional nor ephemeral strings.
        return Abort(content::SPEECH_RECOGNITION_ERROR_NO_MATCH);
      default:
        VLOG(1) << "Received an unknown status code from the speech recognition"
                   "webservice (" << ws_result.status() << ")";
        return Abort(content::SPEECH_RECOGNITION_ERROR_NETWORK);
    }
  }

  SpeechRecognitionResult result;
  if (is_definitive_result) {
    got_last_definitive_result_ = true;
    result.is_provisional = false;
    for (int i = 0; i < ws_result.hypotheses_size(); ++i) {
      const HttpStreamingHypothesis& ws_hypothesis = ws_result.hypotheses(i);
      SpeechRecognitionHypothesis hypothesis;
      DCHECK(ws_hypothesis.has_confidence());
      hypothesis.confidence = ws_hypothesis.confidence();
      DCHECK(ws_hypothesis.has_utterance());
      hypothesis.utterance = UTF8ToUTF16(ws_hypothesis.utterance());
      result.hypotheses.push_back(hypothesis);
    }
  } else {
    result.is_provisional = true;
    string16 transcript;
    if (ws_result.has_provisional())
      transcript.append(UTF8ToUTF16(ws_result.provisional()));
    if (ws_result.has_ephemeral())
      transcript.append(UTF8ToUTF16(ws_result.ephemeral()));
    DCHECK(!transcript.empty());
    result.hypotheses.push_back(SpeechRecognitionHypothesis(transcript, 0.0f));
  }

  // Propagate just actual results.
  if (result.hypotheses.size() > 0)
    delegate()->OnSpeechRecognitionEngineResult(result);

  return state_;
}

GoogleStreamingRemoteEngine::FSMState
GoogleStreamingRemoteEngine::RaiseNoMatchErrorIfGotNoResults(
    const FSMEventArgs& event_args) {
  if (!got_last_definitive_result_) {
    // Provide an empty result to notify that recognition is ended with no
    // errors, yet neither any further results.
    delegate()->OnSpeechRecognitionEngineResult(SpeechRecognitionResult());
  }
  return AbortSilently(event_args);
}

GoogleStreamingRemoteEngine::FSMState
GoogleStreamingRemoteEngine::CloseUpstreamAndWaitForResults(
    const FSMEventArgs&) {
  DCHECK(upstream_fetcher_.get());
  DCHECK(encoder_.get());

  // TODO(primiano): /////////// Remove this after debug stage. ////////////////
  DVLOG(1) <<  "Closing upstream";

  // The encoder requires a non-empty final buffer. So we encode a packet
  // of silence in case encoder had no data already.
  std::vector<short> samples(
      config_.audio_sample_rate * kAudioPacketIntervalMs / 1000);
  scoped_refptr<AudioChunk> dummy_chunk =
      new AudioChunk(reinterpret_cast<uint8*>(&samples[0]),
                     samples.size() * sizeof(short),
                     encoder_->bits_per_sample() / 8);
  encoder_->Encode(*dummy_chunk);
  encoder_->Flush();
  scoped_refptr<AudioChunk> encoded_dummy_data =
      encoder_->GetEncodedDataAndClear();
  DCHECK(!encoded_dummy_data->IsEmpty());
  encoder_.reset();

  upstream_fetcher_->AppendChunkToUpload(encoded_dummy_data->AsString(), true);
  got_last_definitive_result_ = false;
  return STATE_WAITING_DOWNSTREAM_RESULTS;
}

GoogleStreamingRemoteEngine::FSMState
GoogleStreamingRemoteEngine::CloseDownstream(const FSMEventArgs&) {
  DCHECK(!upstream_fetcher_.get());
  DCHECK(downstream_fetcher_.get());

  // TODO(primiano): /////////// Remove this after debug stage. ////////////////
  DVLOG(1) <<  "Closing downstream";
  downstream_fetcher_.reset();
  return STATE_IDLE;
}

GoogleStreamingRemoteEngine::FSMState
GoogleStreamingRemoteEngine::AbortSilently(const FSMEventArgs&) {
  return Abort(content::SPEECH_RECOGNITION_ERROR_NONE);
}

GoogleStreamingRemoteEngine::FSMState
GoogleStreamingRemoteEngine::AbortWithError(const FSMEventArgs&) {
  return Abort(content::SPEECH_RECOGNITION_ERROR_NETWORK);
}

GoogleStreamingRemoteEngine::FSMState GoogleStreamingRemoteEngine::Abort(
    SpeechRecognitionErrorCode error_code) {
  // TODO(primiano): /////////// Remove this after debug stage. ////////////////
  DVLOG(1) << "Aborting with error " << error_code;

  if (error_code != content::SPEECH_RECOGNITION_ERROR_NONE) {
    delegate()->OnSpeechRecognitionEngineError(
        SpeechRecognitionError(error_code));
  }
  downstream_fetcher_.reset();
  upstream_fetcher_.reset();
  encoder_.reset();
  return STATE_IDLE;
}

GoogleStreamingRemoteEngine::FSMState
GoogleStreamingRemoteEngine::DoNothing(const FSMEventArgs&) {
  return state_;
}

GoogleStreamingRemoteEngine::FSMState
GoogleStreamingRemoteEngine::NotFeasible(const FSMEventArgs& event_args) {
  NOTREACHED() << "Unfeasible event " << event_args.event
               << " in state " << state_;
  return state_;
}

std::string GoogleStreamingRemoteEngine::GetAcceptedLanguages() const {
  std::string langs = config_.language;
  if (langs.empty() && url_context_) {
    // If no language is provided then we use the first from the accepted
    // language list. If this list is empty then it defaults to "en-US".
    // Example of the contents of this list: "es,en-GB;q=0.8", ""
    net::URLRequestContext* request_context =
        url_context_->GetURLRequestContext();
    DCHECK(request_context);
    std::string accepted_language_list = request_context->accept_language();
    size_t separator = accepted_language_list.find_first_of(",;");
    if (separator > std::string::npos)
      langs = accepted_language_list.substr(0, separator);
  }
  if (langs.empty())
    langs = "en-US";
  return langs;
}

// TODO(primiano): Is there any utility in the codebase that already does this?
std::string GoogleStreamingRemoteEngine::GenerateRequestKey() const {
  const int64 kKeepLowBytes = GG_LONGLONG(0x00000000FFFFFFFF);
  const int64 kKeepHighBytes = GG_LONGLONG(0xFFFFFFFF00000000);

  // Just keep the least significant bits of timestamp, in order to reduce
  // probability of collisions.
  int64 key = (base::Time::Now().ToInternalValue() & kKeepLowBytes) |
              (base::RandUint64() & kKeepHighBytes);
  return base::HexEncode(reinterpret_cast<void*>(&key), sizeof(key));
}

GoogleStreamingRemoteEngine::FSMEventArgs::FSMEventArgs(FSMEvent event_value)
    : event(event_value) {
}

GoogleStreamingRemoteEngine::FSMEventArgs::~FSMEventArgs() {
}

}  // namespace speech
