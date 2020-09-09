/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/publisher_ads.h"

#include "bat/ads/internal/json_helper.h"

namespace ads {

PublisherAds::PublisherAds() = default;

PublisherAds::PublisherAds(
    const PublisherAds& info) = default;

PublisherAds::~PublisherAds() = default;

std::string PublisherAds::ToJson() const {
  std::string json;
  SaveToJson(*this, &json);
  return json;
}

Result PublisherAds::FromJson(
    const std::string& json,
    std::string* error_description) {
  rapidjson::Document document;
  document.Parse(json.c_str());

  if (document.HasParseError()) {
    if (error_description != nullptr) {
      *error_description = helper::JSON::GetLastError(&document);
    }

    return FAILED;
  }

  if (document.HasMember("publisherAds")) {
    for (const auto& publisher_ad : document["publisherAds"].GetArray()) {
      rapidjson::StringBuffer buffer;
      rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
      if (!publisher_ad.Accept(writer)) {
        continue;
      }

      PublisherAdInfo publisher_ad_info;
      if (publisher_ad_info.FromJson(buffer.GetString()) != SUCCESS) {
        continue;
      }

      items.push_back(publisher_ad_info);
    }
  }

  return SUCCESS;
}

void SaveToJson(
    JsonWriter* writer,
    const PublisherAds& publisher_ads) {
  writer->StartObject();

  writer->String("publisherAds");
  writer->StartArray();
  for (const auto& publisher_ad : publisher_ads.items) {
    SaveToJson(writer, publisher_ad);
  }
  writer->EndArray();

  writer->EndObject();
}

}  // namespace ads
