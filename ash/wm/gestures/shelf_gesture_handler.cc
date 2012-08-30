// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ash/wm/gestures/shelf_gesture_handler.h"

#include "ash/shell.h"
#include "ash/shell_delegate.h"
#include "ash/system/status_area_widget.h"
#include "ash/wm/shelf_layout_manager.h"
#include "ash/wm/shelf_types.h"
#include "ui/aura/window.h"
#include "ui/compositor/layer.h"
#include "ui/compositor/scoped_layer_animation_settings.h"
#include "ui/gfx/transform.h"
#include "ui/views/widget/widget.h"

namespace ash {
namespace internal {

ShelfGestureHandler::ShelfGestureHandler()
    : drag_in_progress_(false) {
}

ShelfGestureHandler::~ShelfGestureHandler() {
}

bool ShelfGestureHandler::ProcessGestureEvent(const ui::GestureEvent& event) {
  Shell* shell = Shell::GetInstance();
  if (!shell->delegate()->IsUserLoggedIn() ||
      shell->delegate()->IsScreenLocked()) {
    // The gestures are disabled in the lock/login screen.
    return false;
  }

  ShelfLayoutManager* shelf = shell->shelf();
  if (event.type() == ui::ET_GESTURE_SCROLL_BEGIN) {
    drag_in_progress_ = true;
    shelf->StartGestureDrag(event);
    return true;
  }

  if (!drag_in_progress_)
    return false;

  if (event.type() == ui::ET_GESTURE_SCROLL_UPDATE) {
    shelf->UpdateGestureDrag(event);
    return true;
  }

  drag_in_progress_ = false;

  if (event.type() == ui::ET_GESTURE_SCROLL_END ||
      event.type() == ui::ET_SCROLL_FLING_START) {
    shelf->HideFromGestureDrag(event);
    return true;
  }

  // Unexpected event. Reset the state and let the event fall through.
  shelf->CancelGestureDrag();
  return false;
}

}  // namespace internal
}  // namespace ash
