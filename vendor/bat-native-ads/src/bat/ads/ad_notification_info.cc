/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/ad_notification_info.h"

#include "bat/ads/internal/logging.h"
#include "bat/ads/internal/json_helper.h"

namespace ads {

AdNotificationInfo::AdNotificationInfo() = default;

AdNotificationInfo::AdNotificationInfo(
    const AdNotificationInfo& info) = default;

AdNotificationInfo::~AdNotificationInfo() = default;

std::string AdNotificationInfo::ToJson() const {
  std::string json;
  SaveToJson(*this, &json);
  return json;
}

Result AdNotificationInfo::FromJson(
    const std::string& json) {
  rapidjson::Document document;
  document.Parse(json.c_str());

  if (document.HasParseError()) {
    BLOG(1, helper::JSON::GetLastError(&document));
    return FAILED;
  }

  if (document.HasMember("uuid")) {
    uuid = document["uuid"].GetString();
  }

  if (document.HasMember("creativeInstanceId")) {
    creative_instance_id = document["creativeInstanceId"].GetString();
  }

  if (document.HasMember("creativeSetId")) {
    creative_set_id = document["creativeSetId"].GetString();
  }

  if (document.HasMember("campaignId")) {
    campaign_id = document["campaignId"].GetString();
  }

  if (document.HasMember("category")) {
    category = document["category"].GetString();
  }

  if (document.HasMember("title")) {
    title = document["title"].GetString();
  }

  if (document.HasMember("body")) {
    body = document["body"].GetString();
  }

  if (document.HasMember("targetUrl")) {
    target_url = document["targetUrl"].GetString();
  }

  if (document.HasMember("geoTarget")) {
    geo_target = document["geoTarget"].GetString();
  }

  return SUCCESS;
}

void SaveToJson(
    JsonWriter* writer,
    const AdNotificationInfo& info) {
  writer->StartObject();

  writer->String("uuid");
  writer->String(info.uuid.c_str());

  writer->String("creativeInstanceId");
  writer->String(info.creative_instance_id.c_str());

  writer->String("creativeSetId");
  writer->String(info.creative_set_id.c_str());

  writer->String("campaignId");
  writer->String(info.campaign_id.c_str());

  writer->String("category");
  writer->String(info.category.c_str());

  writer->String("title");
  writer->String(info.title.c_str());

  writer->String("body");
  writer->String(info.body.c_str());

  writer->String("targetUrl");
  writer->String(info.target_url.c_str());

  writer->String("geoTarget");
  writer->String(info.geo_target.c_str());

  writer->EndObject();
}

}  // namespace ads
