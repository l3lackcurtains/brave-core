/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ad_events/publisher_ad/publisher_ad_event_viewed.h"

#include <stdint.h>

#include "bat/ads/confirmation_type.h"
#include "bat/ads/internal/ads_impl.h"
#include "bat/ads/internal/confirmations/confirmations.h"
#include "bat/ads/internal/logging.h"
#include "bat/ads/internal/time_util.h"

namespace ads {

using std::placeholders::_1;
using std::placeholders::_2;

namespace {
const ConfirmationType kConfirmationType = ConfirmationType::kViewed;
}  // namespace

PublisherAdEventViewed::PublisherAdEventViewed(
    AdsImpl* ads)
    : ads_(ads) {
  DCHECK(ads_);
}

PublisherAdEventViewed::~PublisherAdEventViewed() = default;

void PublisherAdEventViewed::Trigger(
    const PublisherAdInfo& publisher_ad) {
  BLOG(3, "Viewed publisher ad:\n"
      << "  creativeSetId: " << publisher_ad.creative_set_id << "\n"
      << "  creativeInstanceId: " << publisher_ad.creative_instance_id << "\n"
      << "  category: " << publisher_ad.category << "\n"
      << "  creativeUrl: " << publisher_ad.creative_url << "\n"
      << "  size: " << publisher_ad.size << "\n"
      << "  targetUrl: " << publisher_ad.target_url);

  ads_->get_client()->AppendCreativeSetHistoryForCreativeSetId(
      publisher_ad.creative_set_id);

  ads_->get_client()->AppendCampaignHistoryForCampaignId(
      publisher_ad.campaign_id);

  ads_->get_client()->UpdateSeenPublisherAd(
      publisher_ad.creative_instance_id, 1);

  ads_->get_confirmations()->ConfirmAd(publisher_ad, kConfirmationType);

//   ads_->get_ads_client()->FlagPublisherAdWasPreCached(
//       publisher_ad.creative_instance_id, std::bind(&PublisherAdEventViewed::
//           OnFlagPublisherAdWasPreCached, this, _1, _2));
}

///////////////////////////////////////////////////////////////////////////////

// void PublisherAdEventViewed::OnFlagPublisherAdWasPreCached(
//     const std::string& creative_instance_id,
//     const bool was_flagged) {
//   DCHECK(was_flagged);
// }

}  // namespace ads
