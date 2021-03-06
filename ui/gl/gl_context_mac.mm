// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "base/command_line.h"
#include "base/debug/trace_event.h"
#include "base/logging.h"
#include "base/mac/mac_util.h"
#include "base/memory/scoped_ptr.h"
#include "third_party/mesa/MesaLib/include/GL/osmesa.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_context_cgl.h"
#include "ui/gl/gl_context_osmesa.h"
#include "ui/gl/gl_context_stub.h"
#include "ui/gl/gl_implementation.h"
#include "ui/gl/gl_surface.h"
#include "ui/gl/gl_switches.h"
#include "ui/gl/gpu_switching_manager.h"

#if defined(USE_AURA)
#include "ui/gl/gl_context_nsview.h"
#endif

namespace gfx {

class GLShareGroup;

scoped_refptr<GLContext> GLContext::CreateGLContext(
    GLShareGroup* share_group,
    GLSurface* compatible_surface,
    GpuPreference gpu_preference) {
  TRACE_EVENT0("gpu", "GLContext::CreateGLContext");
  switch (GetGLImplementation()) {
    case kGLImplementationDesktopGL:
    case kGLImplementationAppleGL: {
      scoped_refptr<GLContext> context;
#if defined(USE_AURA)
      if (compatible_surface->IsOffscreen())
        context = new GLContextCGL(share_group);
      else
        context = new GLContextNSView(share_group);
#else
      context = new GLContextCGL(share_group);
#endif // USE_AURA
      if (!context->Initialize(compatible_surface, gpu_preference))
        return NULL;

      return context;
    }
    case kGLImplementationOSMesaGL: {
      scoped_refptr<GLContext> context(new GLContextOSMesa(share_group));
      if (!context->Initialize(compatible_surface, gpu_preference))
        return NULL;

      return context;
    }
    case kGLImplementationMockGL:
      return new GLContextStub;
    default:
      NOTREACHED();
      return NULL;
  }
}

bool GLContext::SupportsDualGpus() {
  // We need to know the GL implementation in order to correctly
  // answer whether dual GPUs are supported. This introduces an
  // initialization cycle with GLSurface::InitializeOneOff() which we
  // need to break.
  static bool initialized = false;
  static bool initializing = false;
  static bool supports_dual_gpus = false;

  if (initialized) {
    return supports_dual_gpus;
  } else {
    if (!initializing) {
      initializing = true;
      if (!GLSurface::InitializeOneOff()) {
        return false;
      }
    }
    initialized = true;
  }

  if (!base::mac::IsOSLionOrLater()) {
    return false;
  }

  if (GetGLImplementation() != kGLImplementationDesktopGL) {
    return false;
  }

  // Enumerate all hardware-accelerated renderers. If we find one
  // online and one offline, assume we're on a dual-GPU system.
  GLuint display_mask = static_cast<GLuint>(-1);
  CGLRendererInfoObj renderer_info = NULL;
  GLint num_renderers = 0;

  bool found_online = false;
  bool found_offline = false;

  if (CGLQueryRendererInfo(display_mask,
                           &renderer_info,
                           &num_renderers) != kCGLNoError) {
    return false;
  }

  ScopedCGLRendererInfoObj scoper(renderer_info);

  for (GLint i = 0; i < num_renderers; ++i) {
    GLint accelerated = 0;
    if (CGLDescribeRenderer(renderer_info,
                            i,
                            kCGLRPAccelerated,
                            &accelerated) != kCGLNoError) {
      return false;
    }

    if (!accelerated)
      continue;

    GLint online = 0;
    if (CGLDescribeRenderer(renderer_info,
                            i,
                            kCGLRPOnline,
                            &online) != kCGLNoError) {
      return false;
    }

    if (online) {
      found_online = true;
    } else {
      found_offline = true;
    }
  }

  if (found_online && found_offline) {
    // Only switch GPUs dynamically on recent MacBook Pro models.
    // Otherwise, keep the system on the discrete GPU for the lifetime
    // of the browser process, since switching back and forth causes
    // system stability issues. http://crbug.com/113703
    std::string model;
    int32 model_major, model_minor;
    if (!base::mac::ParseModelIdentifier(base::mac::GetModelIdentifier(),
                                         &model, &model_major, &model_minor)) {
      return false;
    }

    const int kMacBookProFirstDualAMDIntelGPUModel = 8;

    // Do not overwrite commandline switches to honor a user's decision.
    bool forcibly_disable =
        ((model == "MacBookPro") &&
         (model_major < kMacBookProFirstDualAMDIntelGPUModel)) &&
        !CommandLine::ForCurrentProcess()->HasSwitch(switches::kGpuSwitching);

    if (forcibly_disable) {
      GpuSwitchingManager::GetInstance()->ForceUseOfDiscreteGpu();
      return false;
    }

    supports_dual_gpus = true;
  }

  return supports_dual_gpus;
}

}  // namespace gfx
