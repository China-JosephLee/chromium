// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/public/browser/speech_recognition_session_config.h"

namespace {
const uint32 kDefaultMaxHypotheses = 1;
}

namespace content {

SpeechRecognitionSessionConfig::SpeechRecognitionSessionConfig()
    : is_one_shot(true),
      filter_profanities(false),
      max_hypotheses(kDefaultMaxHypotheses),
      event_listener(NULL) {
}

SpeechRecognitionSessionConfig::~SpeechRecognitionSessionConfig() {
}

}  // namespace content
