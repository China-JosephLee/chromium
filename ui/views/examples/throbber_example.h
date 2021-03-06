// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_THROBBER_EXAMPLE_H_
#define UI_VIEWS_EXAMPLES_THROBBER_EXAMPLE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/examples/example_base.h"

namespace views {
class View;

namespace examples {

class ThrobberExample : public ExampleBase {
 public:
  ThrobberExample();
  virtual ~ThrobberExample();

  // Overridden from ExampleBase:
  virtual void CreateExampleView(View* container) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(ThrobberExample);
};

}  // namespace examples
}  // namespace views

#endif  // UI_VIEWS_EXAMPLES_THROBBER_EXAMPLE_H_
