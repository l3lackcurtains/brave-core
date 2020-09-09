/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ad_events/publisher_ad/publisher_ad_event_clicked.h"

#include "bat/ads/confirmation_type.h"
#include "bat/ads/internal/ads_impl.h"
#include "bat/ads/internal/confirmations/confirmations.h"
#include "bat/ads/internal/logging.h"

namespace ads {

namespace {
const ConfirmationType kConfirmationType = ConfirmationType::kClicked;
}  // namespace

PublisherAdEventClicked::PublisherAdEventClicked(
    AdsImpl* ads)
    : ads_(ads) {
  DCHECK(ads_);
}

PublisherAdEventClicked::~PublisherAdEventClicked() = default;

void PublisherAdEventClicked::Trigger(
    const PublisherAdInfo& publisher_ad) {
  BLOG(3, "Clicked publisher ad with " << publisher_ad.creative_instance_id
      << " creative instance id");

  ads_->set_last_shown_ad(publisher_ad);

  ads_->get_confirmations()->ConfirmAd(publisher_ad, kConfirmationType);
}

}  // namespace ads
