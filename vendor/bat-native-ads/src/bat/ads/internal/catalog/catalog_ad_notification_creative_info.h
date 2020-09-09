/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_CATALOG_CATALOG_AD_NOTIFICATION_CREATIVE_INFO_H_
#define BAT_ADS_INTERNAL_CATALOG_CATALOG_AD_NOTIFICATION_CREATIVE_INFO_H_

#include <vector>

#include "bat/ads/internal/catalog/catalog_ad_notification_payload_info.h"
#include "bat/ads/internal/catalog/catalog_creative_info.h"

namespace ads {

struct CatalogAdNotificationCreativeInfo : CatalogCreativeInfo {
  CatalogAdNotificationPayloadInfo payload;
};

using CatalogAdNotificationCreativeList =
    std::vector<CatalogAdNotificationCreativeInfo>;

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_CATALOG_CATALOG_AD_NOTIFICATION_CREATIVE_INFO_H_
