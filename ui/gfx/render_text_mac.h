// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_RENDER_TEXT_MAC_H_
#define UI_GFX_RENDER_TEXT_MAC_H_

#include <ApplicationServices/ApplicationServices.h>

#include <string>
#include <vector>

#include "base/mac/scoped_cftyperef.h"
#include "ui/gfx/render_text.h"

namespace gfx {

// RenderTextMac is the Mac implementation of RenderText that uses CoreText for
// layout and Skia for drawing.
//
// Note: The current implementation only supports drawing and sizing the text,
//       but not text selection or cursor movement.
class RenderTextMac : public RenderText {
 public:
  RenderTextMac();
  virtual ~RenderTextMac();

  // Overridden from RenderText:
  virtual Size GetStringSize() OVERRIDE;
  virtual int GetBaseline() OVERRIDE;
  virtual SelectionModel FindCursorPosition(const Point& point) OVERRIDE;
  virtual std::vector<FontSpan> GetFontSpansForTesting() OVERRIDE;

 protected:
  // Overridden from RenderText:
  virtual SelectionModel AdjacentCharSelectionModel(
      const SelectionModel& selection,
      VisualCursorDirection direction) OVERRIDE;
  virtual SelectionModel AdjacentWordSelectionModel(
      const SelectionModel& selection,
      VisualCursorDirection direction) OVERRIDE;
  virtual void GetGlyphBounds(size_t index,
                              ui::Range* xspan,
                              int* height) OVERRIDE;
  virtual std::vector<Rect> GetSubstringBounds(ui::Range range) OVERRIDE;
  virtual bool IsCursorablePosition(size_t position) OVERRIDE;
  virtual void ResetLayout() OVERRIDE;
  virtual void EnsureLayout() OVERRIDE;
  virtual void DrawVisualText(Canvas* canvas) OVERRIDE;

 private:
  struct TextRun {
    CTRunRef ct_run;
    SkPoint origin;
    std::vector<uint16> glyphs;
    std::vector<SkPoint> glyph_positions;
    SkScalar width;
    std::string font_name;
    int font_style;
    SkScalar text_size;
    SkColor foreground;
    StyleRange style;

    TextRun();
    ~TextRun();
  };

  // Applies RenderText styles to |attr_string| with the given |ct_font|.
  void ApplyStyles(CFMutableAttributedStringRef attr_string, CTFontRef ct_font);

  // Updates |runs_| based on |line_| and sets |runs_valid_| to true.
  void ComputeRuns();

  // The Core Text line of text. Created by |EnsureLayout()|.
  base::mac::ScopedCFTypeRef<CTLineRef> line_;

  // Array to hold CFAttributedString attributes that allows Core Text to hold
  // weak references to them without leaking.
  base::mac::ScopedCFTypeRef<CFMutableArrayRef> attributes_;

  // Visual dimensions of the text. Computed by |EnsureLayout()|.
  Size string_size_;

  // Common baseline for this line of text. Computed by |EnsureLayout()|.
  SkScalar common_baseline_;

  // Visual text runs. Only valid if |runs_valid_| is true. Computed by
  // |ComputeRuns()|.
  std::vector<TextRun> runs_;

  // Indicates that |runs_| are valid, set by |ComputeRuns()|.
  bool runs_valid_;

  DISALLOW_COPY_AND_ASSIGN(RenderTextMac);
};

}  // namespace gfx

#endif  // UI_GFX_RENDER_TEXT_MAC_H_
