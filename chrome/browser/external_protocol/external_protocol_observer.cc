// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/external_protocol/external_protocol_observer.h"

#include "chrome/browser/external_protocol/external_protocol_handler.h"

using content::WebContents;

int ExternalProtocolObserver::kUserDataKey;

ExternalProtocolObserver::ExternalProtocolObserver(WebContents* web_contents)
    : content::WebContentsObserver(web_contents) {
}

ExternalProtocolObserver::~ExternalProtocolObserver() {
}

void ExternalProtocolObserver::DidGetUserGesture() {
  ExternalProtocolHandler::PermitLaunchUrl();
}
