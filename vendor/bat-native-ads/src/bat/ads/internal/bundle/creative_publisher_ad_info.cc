/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/bundle/creative_publisher_ad_info.h"

namespace ads {

CreativePublisherAdInfo::CreativePublisherAdInfo() = default;

CreativePublisherAdInfo::CreativePublisherAdInfo(
    const CreativePublisherAdInfo& info) = default;

CreativePublisherAdInfo::~CreativePublisherAdInfo() = default;

bool CreativePublisherAdInfo::operator==(
    const CreativePublisherAdInfo& rhs) const {
  return creative_url == rhs.creative_url &&
      size == rhs.size &&
      channels == rhs.channels;
}

bool CreativePublisherAdInfo::operator!=(
    const CreativePublisherAdInfo& rhs) const {
  return !(*this == rhs);
}

}  // namespace ads
