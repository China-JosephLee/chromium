// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/resource/resource_bundle.h"

#include <string>

#include "base/file_path.h"
#include "base/file_util.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "base/stringprintf.h"
#include "ui/base/ui_base_paths.h"
#include "ui/base/resource/resource_handle.h"

namespace ui {

void ResourceBundle::LoadCommonResources() {
  FilePath path;
  PathService::Get(ui::DIR_RESOURCE_PAKS_ANDROID, &path);
  AddDataPackFromPath(path.AppendASCII("chrome.pak"),
                      SCALE_FACTOR_100P);
  AddDataPackFromPath(path.AppendASCII("theme_resources_100_percent.pak"),
                      SCALE_FACTOR_100P);
  AddDataPackFromPath(path.AppendASCII("ui_resources_100_percent.pak"),
                      SCALE_FACTOR_100P);
}

gfx::Image& ResourceBundle::GetNativeImageNamed(int resource_id, ImageRTL rtl) {
  // Flipped image is not used on Android.
  DCHECK_EQ(rtl, RTL_DISABLED);
  return GetImageNamed(resource_id);
}

}
