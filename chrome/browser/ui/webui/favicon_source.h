// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_FAVICON_SOURCE_H_
#define CHROME_BROWSER_UI_WEBUI_FAVICON_SOURCE_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/favicon/favicon_service.h"
#include "chrome/browser/ui/webui/chrome_url_data_manager.h"

class Profile;

// FaviconSource is the gateway between network-level chrome:
// requests for favicons and the history backend that serves these.
class FaviconSource : public ChromeURLDataManager::DataSource {
 public:
  // Defines the type of icon the FaviconSource will provide.
  enum IconType {
    FAVICON,
    // Any available icon in the priority of TOUCH_ICON_PRECOMPOSED, TOUCH_ICON,
    // FAVICON, and default favicon.
    ANY
  };

  // |type| is the type of icon this FaviconSource will provide.
  FaviconSource(Profile* profile, IconType type);

  // Constructor allowing the source name to be specified.
  FaviconSource(Profile* profile,
                IconType type,
                const std::string& source_name);

  // Called when the network layer has requested a resource underneath
  // the path we registered.
  virtual void StartDataRequest(const std::string& path,
                                bool is_incognito,
                                int request_id) OVERRIDE;

  virtual std::string GetMimeType(const std::string&) const OVERRIDE;

  virtual bool ShouldReplaceExistingSource() const OVERRIDE;

 protected:
  virtual ~FaviconSource();

  Profile* profile_;

 private:

  struct IconRequest {
    IconRequest()
      : request_id(0),
        pixel_size(16),
        scale_factor(ui::SCALE_FACTOR_NONE) {
    }
    IconRequest(int id, int size, ui::ScaleFactor scale)
      : request_id(id),
        pixel_size(size),
        scale_factor(scale) {
    }
    int request_id;
    int pixel_size;
    ui::ScaleFactor scale_factor;
  };

  void Init(Profile* profile, IconType type);

  // Called when favicon data is available from the history backend.
  void OnFaviconDataAvailable(
      FaviconService::Handle request_handle,
      const history::FaviconBitmapResult& bitmap_result);

  // Sends the default favicon.
  void SendDefaultResponse(const IconRequest& request);

  CancelableRequestConsumerTSimple<IconRequest> cancelable_consumer_;

  // Raw PNG representation of the favicon to show when the favicon
  // database doesn't have a favicon for a webpage.
  // 16x16
  scoped_refptr<base::RefCountedMemory> default_favicon_;
  // 32x32
  scoped_refptr<base::RefCountedMemory> default_favicon_large_;

  // The history::IconTypes of icon that this FaviconSource handles.
  int icon_types_;

  DISALLOW_COPY_AND_ASSIGN(FaviconSource);
};

#endif  // CHROME_BROWSER_UI_WEBUI_FAVICON_SOURCE_H_
