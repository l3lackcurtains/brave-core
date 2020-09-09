/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_CATALOG_CATALOG_PUBLISHER_AD_CREATIVE_INFO_H_
#define BAT_ADS_INTERNAL_CATALOG_CATALOG_PUBLISHER_AD_CREATIVE_INFO_H_

#include <vector>

#include "bat/ads/internal/catalog/catalog_creative_info.h"
#include "bat/ads/internal/catalog/catalog_publisher_ad_channel_info.h"
#include "bat/ads/internal/catalog/catalog_publisher_ad_payload_info.h"

namespace ads {

struct CatalogPublisherAdCreativeInfo : CatalogCreativeInfo {
  CatalogPublisherAdCreativeInfo();
  CatalogPublisherAdCreativeInfo(
      const CatalogPublisherAdCreativeInfo& info);
  ~CatalogPublisherAdCreativeInfo();

  CatalogPublisherAdChannelList channels;
  CatalogPublisherAdPayloadInfo payload;
};

using CatalogPublisherAdCreativeList =
    std::vector<CatalogPublisherAdCreativeInfo>;

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_CATALOG_CATALOG_PUBLISHER_AD_CREATIVE_INFO_H_
