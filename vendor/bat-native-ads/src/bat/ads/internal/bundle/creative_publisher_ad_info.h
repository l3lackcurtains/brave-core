/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_BUNDLE_CREATIVE_PUBLISHER_AD_INFO_H_
#define BAT_ADS_INTERNAL_BUNDLE_CREATIVE_PUBLISHER_AD_INFO_H_

#include <map>
#include <string>
#include <vector>

#include "bat/ads/internal/bundle/creative_ad_info.h"

namespace ads {

struct CreativePublisherAdInfo : CreativeAdInfo {
  CreativePublisherAdInfo();
  CreativePublisherAdInfo(
      const CreativePublisherAdInfo& info);
  ~CreativePublisherAdInfo();

  bool operator==(
      const CreativePublisherAdInfo& rhs) const;

  bool operator!=(
      const CreativePublisherAdInfo& rhs) const;

  std::string creative_url;
  std::string size;
  std::vector<std::string> channels;
};

using CreativePublisherAdList = std::vector<CreativePublisherAdInfo>;
using CreativePublisherAdMap = std::map<std::string, CreativePublisherAdList>;

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_BUNDLE_CREATIVE_PUBLISHER_AD_INFO_H_
