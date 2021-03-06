// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Base interface used by NPChannelBase and implemented by NPObjectProxy and
// NPObjectStub.

#ifndef CONTENT_COMMON_NPOBJECT_BASE_H_
#define CONTENT_COMMON_NPOBJECT_BASE_H_

#include "ipc/ipc_listener.h"
#include "third_party/npapi/bindings/npruntime.h"

struct NPObject;

class NPObjectBase {
 public:
  virtual ~NPObjectBase() {}

  // Returns the underlying NPObject handled by this NPObjectBase instance.
  virtual NPObject* GetUnderlyingNPObject() = 0;

  // Returns the channel listener for this NPObjectBase instance.
  virtual IPC::Listener* GetChannelListener() = 0;
};

#endif  // CONTENT_COMMON_NPOBJECT_BASE_H_
