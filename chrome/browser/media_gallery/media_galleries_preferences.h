// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERY_MEDIA_GALLERIES_PREFERENCES_H_
#define CHROME_BROWSER_MEDIA_GALLERY_MEDIA_GALLERIES_PREFERENCES_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/string16.h"
#include "chrome/browser/profiles/profile_keyed_service.h"

class PrefService;
class Profile;

namespace base {
class DictionaryValue;
}

namespace extensions {
class Extension;
class ExtensionPrefs;
}

namespace chrome {

typedef uint64 MediaGalleryPrefId;
const MediaGalleryPrefId kInvalidMediaGalleryPrefId = 0;

struct MediaGalleryPermission {
  MediaGalleryPrefId pref_id;
  bool has_permission;
};

struct MediaGalleryPrefInfo {
  enum Type {
    kAutoDetected,  // Auto added to the list of galleries.
    kUserAdded,     // Explicitly added by the user.
    kBlackListed,   // Auto added but then removed by the user.
  };

  MediaGalleryPrefInfo();
  ~MediaGalleryPrefInfo();

  // The ID that identifies this gallery in this Profile.
  MediaGalleryPrefId pref_id;

  // The user-visible name of this gallery.
  string16 display_name;

  // A string which uniquely and persistently identifies the device that the
  // gallery lives on.
  std::string device_id;

  // The root of the gallery, relative to the root of the device.
  FilePath path;

  // The type of gallery.
  Type type;
};

typedef std::map<MediaGalleryPrefId, MediaGalleryPrefInfo>
    MediaGalleriesPrefInfoMap;

// A class to manage the media gallery preferences.  There is one instance per
// user profile.
class MediaGalleriesPreferences : public ProfileKeyedService {
 public:

  explicit MediaGalleriesPreferences(Profile* profile);
  virtual ~MediaGalleriesPreferences();

  // Builds |remembered_galleries_| from the persistent store.
  void InitFromPrefs();

  // Lookup a media gallery and fill in information about it and return true.
  // If the media gallery does not already exist, fill in as much of the
  // MediaGalleryPrefInfo struct as we can and return false.
  // TODO(vandebo) figure out if we want this to be async, in which case:
  // void LookUpGalleryByPath(FilePath&path, callback(const MediaGalleryInfo&))
  bool LookUpGalleryByPath(const FilePath& path,
                           MediaGalleryPrefInfo* gallery) const;

  // Teaches the registry about a new gallery.
  MediaGalleryPrefId AddGallery(const std::string& device_id,
                                const string16& display_name,
                                const FilePath& path,
                                bool user_added);

  // Deprecated: Teach the registry about a user added registry simply from
  // the path.
  // TODO(vandebo) remove once webui/options doesn't use this anymore.
  MediaGalleryPrefId AddGalleryByPath(const FilePath& path);

  // Removes the gallery identified by |id| from the store.
  void ForgetGalleryById(MediaGalleryPrefId id);

  std::set<MediaGalleryPrefId> GalleriesForExtension(
      const extensions::Extension& extension) const;

  void SetGalleryPermissionForExtension(const extensions::Extension& extension,
                                        MediaGalleryPrefId pref_id,
                                        bool has_permission);

  const MediaGalleriesPrefInfoMap& known_galleries() const {
    return known_galleries_;
  }

  // ProfileKeyedService implementation:
  virtual void Shutdown() OVERRIDE;

  static void RegisterUserPrefs(PrefService* prefs);

  // Returns true if the media gallery UI is turned on.
  static bool UserInteractionIsEnabled();

 protected:
  // For testing.
  static string16 ComputeDisplayName(const FilePath& path);

 private:
  // The profile that owns |this|.
  Profile* profile_;

  // An in-memory cache of known galleries.
  MediaGalleriesPrefInfoMap known_galleries_;

  extensions::ExtensionPrefs* GetExtensionPrefs() const;

  DISALLOW_COPY_AND_ASSIGN(MediaGalleriesPreferences);
};

}  // namespace chrome

#endif  // CHROME_BROWSER_MEDIA_GALLERY_MEDIA_GALLERIES_PREFERENCES_H_
