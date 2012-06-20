// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_ANDROID_CONTENT_VIEW_IMPL_H_
#define CONTENT_BROWSER_ANDROID_CONTENT_VIEW_IMPL_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/public/browser/android/content_view.h"
#include "content/public/browser/notification_observer.h"

class ContentViewClient;

namespace content {

// TODO(jrg): this is a shell.  Upstream the rest.
class ContentViewImpl : public ContentView,
                        public NotificationObserver {
 public:
  ContentViewImpl(JNIEnv* env,
                  jobject obj,
                  WebContents* web_contents);
  virtual void Destroy(JNIEnv* env, jobject obj);

  // --------------------------------------------------------------------------
  // Methods called from native code
  // --------------------------------------------------------------------------

  WebContents* web_contents() const { return web_contents_; }

 private:
  // NotificationObserver implementation.
  virtual void Observe(int type,
                       const NotificationSource& source,
                       const NotificationDetails& details) OVERRIDE;

  // --------------------------------------------------------------------------
  // Private methods that call to Java via JNI
  // --------------------------------------------------------------------------
  virtual ~ContentViewImpl();

  // --------------------------------------------------------------------------
  // Other private methods and data
  // --------------------------------------------------------------------------

  // Reference to the current WebContents used to determine how and what to
  // display in the ContentView.
  WebContents* web_contents_;

  DISALLOW_COPY_AND_ASSIGN(ContentViewImpl);
};

bool RegisterContentView(JNIEnv* env);

};  // namespace content

#endif  // CONTENT_BROWSER_ANDROID_CONTENT_VIEW_IMPL_H_
