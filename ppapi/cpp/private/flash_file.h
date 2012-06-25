// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_PRIVATE_FLASH_FILE_H_
#define PPAPI_CPP_PRIVATE_FLASH_FILE_H_

#include <string>
#include <vector>

#include "ppapi/c/private/ppb_flash_file.h"

namespace pp {

class FileRef;
class InstanceHandle;

namespace flash {

// FileModuleLocal -------------------------------------------------------------

class FileModuleLocal {
 public:
  // Returns true if the required interface is available.
  static bool IsAvailable();

  static bool CreateThreadAdapterForInstance(const InstanceHandle& instance);
  static void ClearThreadAdapterForInstance(const InstanceHandle& instance);

  // Returns |PP_kInvalidFileHandle| on error.
  static PP_FileHandle OpenFile(const InstanceHandle& instance,
                                const std::string& path,
                                int32_t mode);
  static bool RenameFile(const InstanceHandle& instance,
                         const std::string& path_from,
                         const std::string& path_to);
  static bool DeleteFileOrDir(const InstanceHandle& instance,
                              const std::string& path,
                              bool recursive);
  static bool CreateDir(const InstanceHandle& instance,
                        const std::string& path);
  static bool QueryFile(const InstanceHandle& instance,
                        const std::string& path,
                        PP_FileInfo* info);
  // Note that, unlike the C interface, no |FreeDirContents()| is needed.
  static bool GetDirContents(const InstanceHandle& instance,
                             const std::string& path,
                             std::vector<PP_DirEntry_Dev>* dir_contents);

  // Returns true if |CreateTemporaryFile()| is supported.
  // TODO(viettrungluu): Remove this sometime after M21 ships to Stable?
  static bool IsCreateTemporaryFileAvailable();
  // Returns |PP_kInvalidFileHandle| on error.
  static PP_FileHandle CreateTemporaryFile(const InstanceHandle& instance);
};

// FileFileRef -----------------------------------------------------------------

class FileFileRef {
 public:
  // Returns true if the required interface is available.
  static bool IsAvailable();

  // Returns |PP_kInvalidFileHandle| on error.
  static PP_FileHandle OpenFile(const pp::FileRef& resource,
                                int32_t mode);
  static bool QueryFile(const pp::FileRef& resource,
                        PP_FileInfo* info);
};

}  // namespace flash
}  // namespace pp

#endif  // PPAPI_CPP_PRIVATE_FLASH_FILE_H_
