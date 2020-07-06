/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/ui/brave_custom_notification/notification_background_painter.h"

#include "third_party/skia/include/core/SkPath.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/skia_util.h"

namespace brave_custom_notification {

NotificationBackgroundPainter::NotificationBackgroundPainter(int top_radius,
                                                             int bottom_radius,
                                                             SkColor color)
    : top_radius_(SkIntToScalar(top_radius)),
      bottom_radius_(SkIntToScalar(bottom_radius)),
      color_(color) {}

NotificationBackgroundPainter::~NotificationBackgroundPainter() = default;

gfx::Size NotificationBackgroundPainter::GetMinimumSize() const {
  return gfx::Size();
}

void NotificationBackgroundPainter::Paint(gfx::Canvas* canvas,
                                          const gfx::Size& size) {
  SkPath path;
  SkScalar radii[8] = {top_radius_,    top_radius_,    top_radius_,
                       top_radius_,    bottom_radius_, bottom_radius_,
                       bottom_radius_, bottom_radius_};
  gfx::Rect rect(size);
  rect.Inset(insets_);
  path.addRoundRect(gfx::RectToSkRect(rect), radii);

  cc::PaintFlags flags;
  flags.setAntiAlias(true);
  flags.setStyle(cc::PaintFlags::kFill_Style);
  flags.setColor(color_);
  canvas->DrawPath(path, flags);
}

}  // namespace message_center
