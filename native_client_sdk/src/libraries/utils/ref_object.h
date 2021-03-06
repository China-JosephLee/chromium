/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef LIBRARIES_UTILS_REF_OBJECT
#define LIBRARIES_UTILS_REF_OBJECT

#include <stdlib.h>
#include "pthread.h"

class RefObject {
 public:
  RefObject() {
    ref_count_ = 1;
    pthread_mutex_init(&lock_, NULL);
  }

  int RefCount() const { return ref_count_; }

  void Acquire() {
    ref_count_++;
  }
  bool Release() {
    if (--ref_count_ == 0) {
      delete this;
      return false;
    }
    return true;
  }

 protected:
  virtual ~RefObject() {
    pthread_mutex_destroy(&lock_);
  }

  pthread_mutex_t lock_;

 private:
  int ref_count_;
};

#endif  // LIBRARIES_UTILS_REF_OBJECT
