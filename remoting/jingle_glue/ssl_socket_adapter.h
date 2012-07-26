// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_JINGLE_GLUE_SSL_SOCKET_ADAPTER_H_
#define REMOTING_JINGLE_GLUE_SSL_SOCKET_ADAPTER_H_

#include "base/memory/scoped_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"
#include "net/base/net_errors.h"
#include "net/base/net_log.h"
#include "net/socket/ssl_client_socket.h"
#include "net/socket/stream_socket.h"
#include "third_party/libjingle/source/talk/base/asyncsocket.h"
#include "third_party/libjingle/source/talk/base/ssladapter.h"

namespace net {
class CertVerifier;
class TransportSecurityState;
}  // namespace net

namespace remoting {

class SSLSocketAdapter;

// TODO(sergeyu): Write unittests for this code!

// This class provides a wrapper to libjingle's talk_base::AsyncSocket that
// implements Chromium's net::StreamSocket interface.  It's used by
// SSLSocketAdapter to enable Chromium's SSL implementation to work over
// libjingle's socket class.
class TransportSocket : public net::StreamSocket, public sigslot::has_slots<> {
 public:
  TransportSocket(talk_base::AsyncSocket* socket,
                  SSLSocketAdapter *ssl_adapter);
  virtual ~TransportSocket();

  void set_addr(const talk_base::SocketAddress& addr) {
    addr_ = addr;
  }

  // net::StreamSocket implementation.
  virtual int Connect(const net::CompletionCallback& callback) OVERRIDE;
  virtual void Disconnect() OVERRIDE;
  virtual bool IsConnected() const OVERRIDE;
  virtual bool IsConnectedAndIdle() const OVERRIDE;
  virtual int GetPeerAddress(net::IPEndPoint* address) const OVERRIDE;
  virtual int GetLocalAddress(net::IPEndPoint* address) const OVERRIDE;
  virtual const net::BoundNetLog& NetLog() const OVERRIDE;
  virtual void SetSubresourceSpeculation() OVERRIDE;
  virtual void SetOmniboxSpeculation() OVERRIDE;
  virtual bool WasEverUsed() const OVERRIDE;
  virtual bool UsingTCPFastOpen() const OVERRIDE;
  virtual int64 NumBytesRead() const OVERRIDE;
  virtual base::TimeDelta GetConnectTimeMicros() const OVERRIDE;
  virtual bool WasNpnNegotiated() const OVERRIDE;
  virtual net::NextProto GetNegotiatedProtocol() const OVERRIDE;
  virtual bool GetSSLInfo(net::SSLInfo* ssl_info) OVERRIDE;

  // net::Socket implementation.
  virtual int Read(net::IOBuffer* buf, int buf_len,
                   const net::CompletionCallback& callback) OVERRIDE;
  virtual int Write(net::IOBuffer* buf, int buf_len,
                    const net::CompletionCallback& callback) OVERRIDE;
  virtual bool SetReceiveBufferSize(int32 size) OVERRIDE;
  virtual bool SetSendBufferSize(int32 size) OVERRIDE;

 private:
  friend class SSLSocketAdapter;

  void OnReadEvent(talk_base::AsyncSocket* socket);
  void OnWriteEvent(talk_base::AsyncSocket* socket);

  // Holds the user's completion callback when Write and Read are called.
  net::CompletionCallback read_callback_;
  net::CompletionCallback write_callback_;

  scoped_refptr<net::IOBuffer> read_buffer_;
  int read_buffer_len_;
  scoped_refptr<net::IOBuffer> write_buffer_;
  int write_buffer_len_;

  net::BoundNetLog net_log_;

  talk_base::AsyncSocket *socket_;
  talk_base::SocketAddress addr_;

  bool was_used_to_convey_data_;

  DISALLOW_COPY_AND_ASSIGN(TransportSocket);
};

// This provides a talk_base::AsyncSocketAdapter interface around Chromium's
// net::SSLClientSocket class.  This allows remoting to use Chromium's SSL
// implementation instead of OpenSSL.
class SSLSocketAdapter : public talk_base::SSLAdapter {
 public:
  explicit SSLSocketAdapter(talk_base::AsyncSocket* socket);
  virtual ~SSLSocketAdapter();

  // StartSSL returns 0 if successful, or non-zero on failure.
  // If StartSSL is called while the socket is closed or connecting, the SSL
  // negotiation will begin as soon as the socket connects.
  //
  // restartable is not implemented, and must be set to false.
  virtual int StartSSL(const char* hostname, bool restartable) OVERRIDE;

  // Create the default SSL adapter for this platform.
  static SSLSocketAdapter* Create(AsyncSocket* socket);

  virtual int Send(const void* pv, size_t cb) OVERRIDE;
  virtual int Recv(void* pv, size_t cb) OVERRIDE;

 private:
  friend class TransportSocket;

  enum SSLState {
    SSLSTATE_NONE,
    SSLSTATE_WAIT,
    SSLSTATE_CONNECTED,
    SSLSTATE_ERROR,
  };

  void OnConnected(int result);
  void OnRead(int result);
  void OnWritten(int result);

  void DoWrite();

  virtual void OnConnectEvent(talk_base::AsyncSocket* socket) OVERRIDE;

  int BeginSSL();

  bool ignore_bad_cert_;
  std::string hostname_;
  TransportSocket* transport_socket_;

  // |cert_verifier_| must be defined before |ssl_socket_|, so that
  // it's destroyed after |ssl_socket_|.
  scoped_ptr<net::CertVerifier> cert_verifier_;
  scoped_ptr<net::TransportSecurityState> transport_security_state_;
  scoped_ptr<net::SSLClientSocket> ssl_socket_;

  SSLState ssl_state_;

  bool read_pending_;
  scoped_refptr<net::GrowableIOBuffer> read_buffer_;

  bool write_pending_;
  scoped_refptr<net::DrainableIOBuffer> write_buffer_;

  DISALLOW_COPY_AND_ASSIGN(SSLSocketAdapter);
};

}  // namespace remoting

#endif  // REMOTING_JINGLE_GLUE_SSL_SOCKET_ADAPTER_H_
