// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/ui/webui/options2/chromeos/set_wallpaper_options_handler2.h"

#include "ash/desktop_background/desktop_background_resources.h"
#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/metrics/histogram.h"
#include "base/path_service.h"
#include "base/string_number_conversions.h"
#include "base/string_util.h"
#include "base/values.h"
#include "chrome/browser/chromeos/login/user_manager.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/views/window.h"
#include "chrome/browser/ui/webui/web_ui_util.h"
#include "chrome/common/chrome_notification_types.h"
#include "content/public/browser/notification_service.h"
#include "content/public/browser/web_ui.h"
#include "grit/generated_resources.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/views/widget/widget.h"

namespace chromeos {
namespace options2 {

SetWallpaperOptionsHandler::SetWallpaperOptionsHandler()
    : ALLOW_THIS_IN_INITIALIZER_LIST(weak_factory_(this)) {
}

SetWallpaperOptionsHandler::~SetWallpaperOptionsHandler() {
}

void SetWallpaperOptionsHandler::GetLocalizedValues(
    DictionaryValue* localized_strings) {
  DCHECK(localized_strings);
  localized_strings->SetString("setWallpaperPage",
      l10n_util::GetStringUTF16(IDS_OPTIONS_SET_WALLPAPER_DIALOG_TITLE));
  localized_strings->SetString("setWallpaperPageDescription",
      l10n_util::GetStringUTF16(IDS_OPTIONS_SET_WALLPAPER_DIALOG_TEXT));
}

void SetWallpaperOptionsHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback("onSetWallpaperPageInitialized",
      base::Bind(&SetWallpaperOptionsHandler::HandlePageInitialized,
                 base::Unretained(this)));
  web_ui()->RegisterMessageCallback("onSetWallpaperPageShown",
      base::Bind(&SetWallpaperOptionsHandler::HandlePageShown,
                 base::Unretained(this)));
  web_ui()->RegisterMessageCallback("selectWallpaper",
      base::Bind(&SetWallpaperOptionsHandler::HandleSelectImage,
                 base::Unretained(this)));
}

void SetWallpaperOptionsHandler::SendDefaultImages() {
  ListValue image_urls;

  for (int i = 0; i < ash::GetWallpaperCount(); ++i) {
    image_urls.Append(Value::CreateStringValue(
        web_ui_util::GetImageDataUrl(ash::GetWallpaperThumbnail(i))));
  }

  web_ui()->CallJavascriptFunction("SetWallpaperOptions.setDefaultImages",
                                   image_urls);
}

void SetWallpaperOptionsHandler::HandlePageInitialized(
    const base::ListValue* args) {
  DCHECK(args && args->empty());

  SendDefaultImages();
}

void SetWallpaperOptionsHandler::HandlePageShown(const base::ListValue* args) {
  DCHECK(args && args->empty());
  chromeos::UserManager* user_manager = chromeos::UserManager::Get();
  const chromeos::User& user = user_manager->GetLoggedInUser();
  DCHECK(!user.email().empty());
  int index = user_manager->GetUserWallpaper(user.email());
  DCHECK(index >=0 && index < ash::GetWallpaperCount());
  base::FundamentalValue index_value(index);
  web_ui()->CallJavascriptFunction("SetWallpaperOptions.setSelectedImage",
                                   index_value);
}

void SetWallpaperOptionsHandler::HandleSelectImage(const ListValue* args) {
  std::string image_index_string;
  int image_index;
  if (!args ||
      args->GetSize() != 1 ||
      !args->GetString(0, &image_index_string) ||
      !base::StringToInt(image_index_string, &image_index) ||
      image_index < 0 || image_index >= ash::GetWallpaperCount())
    NOTREACHED();

  UserManager* user_manager = UserManager::Get();
  const User& user = user_manager->GetLoggedInUser();
  DCHECK(!user.email().empty());
  user_manager->SaveWallpaperDefaultIndex(user.email(), image_index);
}

gfx::NativeWindow SetWallpaperOptionsHandler::GetBrowserWindow() const {
  Browser* browser =
      BrowserList::FindBrowserWithProfile(Profile::FromWebUI(web_ui()));
  if (!browser)
    return NULL;
  return browser->window()->GetNativeHandle();
}

}  // namespace options2
}  // namespace chromeos
