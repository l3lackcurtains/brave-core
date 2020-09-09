/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/catalog/catalog_state.h"

#include "url/gurl.h"
#include "bat/ads/internal/logging.h"
#include "bat/ads/internal/json_helper.h"

namespace ads {

namespace {
const uint64_t kDefaultCatalogPing = 2 * base::Time::kSecondsPerHour;
}  // namespace

CatalogState::CatalogState() = default;

CatalogState::CatalogState(
    const CatalogState& state) = default;

CatalogState::~CatalogState() = default;

Result CatalogState::FromJson(
    const std::string& json,
    const std::string& json_schema) {
  rapidjson::Document document;
  document.Parse(json.c_str());

  const Result result = helper::JSON::Validate(&document, json_schema);
  if (result != SUCCESS) {
    BLOG(1, helper::JSON::GetLastError(&document));
    return result;
  }

  std::string new_catalog_id;
  uint64_t new_version = 0;
  uint64_t new_ping = kDefaultCatalogPing * base::Time::kMillisecondsPerSecond;
  CatalogCampaignList new_campaigns;
  CatalogIssuersInfo new_issuers;

  new_catalog_id = document["catalogId"].GetString();

  new_version = document["version"].GetUint64();
  if (new_version != 4) {
    return SUCCESS;
  }

  new_ping = document["ping"].GetUint64();

  // Campaigns
  for (const auto& campaign_node : document["campaigns"].GetArray()) {
    CatalogCampaignInfo campaign;

    campaign.id = campaign_node["campaignId"].GetString();
    campaign.priority = campaign_node["priority"].GetUint();
    campaign.start_at = campaign_node["startAt"].GetString();
    campaign.end_at = campaign_node["endAt"].GetString();
    campaign.daily_cap = campaign_node["dailyCap"].GetUint();
    campaign.advertiser_id = campaign_node["advertiserId"].GetString();

    // Geo targets
    for (const auto& geo_target_node : campaign_node["geoTargets"].GetArray()) {
      CatalogGeoTargetInfo geo_target;
      geo_target.code = geo_target_node["code"].GetString();
      geo_target.name = geo_target_node["name"].GetString();

      campaign.geo_targets.push_back(geo_target);
    }

    // Day parts
    for (const auto& day_part_node : campaign_node["dayParts"].GetArray()) {
      CatalogDayPartInfo day_part;

      day_part.dow = day_part_node["dow"].GetString();
      day_part.start_minute = day_part_node["startMinute"].GetUint();
      day_part.end_minute = day_part_node["endMinute"].GetUint();

      campaign.day_parts.push_back(day_part);
    }

    // Creative sets
    for (const auto& creative_set_node :
        campaign_node["creativeSets"].GetArray()) {
      CatalogCreativeSetInfo creative_set;

      creative_set.id = creative_set_node["creativeSetId"].GetString();

      creative_set.per_day = creative_set_node["perDay"].GetUint();

      creative_set.total_max = creative_set_node["totalMax"].GetUint();

      // Segments
      const auto segment_nodes = creative_set_node["segments"].GetArray();
      if (segment_nodes.Size() == 0) {
        continue;
      }

      for (const auto& segment_node : segment_nodes) {
        CatalogSegmentInfo segment;

        segment.code = segment_node["code"].GetString();
        segment.name = segment_node["name"].GetString();

        creative_set.segments.push_back(segment);
      }

      // Oses
      for (const auto& os_node : creative_set_node["oses"].GetArray()) {
        CatalogOsInfo os;

        os.code = os_node["code"].GetString();
        os.name = os_node["name"].GetString();

        creative_set.oses.push_back(os);
      }

      // Conversions
      for (const auto& conversion_node :
          creative_set_node["conversions"].GetArray()) {
        AdConversionInfo ad_conversion;

        ad_conversion.creative_set_id = creative_set.id;
        ad_conversion.type = conversion_node["type"].GetString();
        ad_conversion.url_pattern = conversion_node["urlPattern"].GetString();
        ad_conversion.observation_window =
            conversion_node["observationWindow"].GetUint();

        base::Time end_at_time;
        if (!base::Time::FromUTCString(campaign.end_at.c_str(), &end_at_time)) {
          BLOG(1, "Creative set id " << creative_set.id << " has an invalid "
              "endAt timestamp");

          continue;
        }

        const base::Time expiry_time = end_at_time +
            base::TimeDelta::FromDays(ad_conversion.observation_window);
        ad_conversion.expiry_timestamp =
            static_cast<int64_t>(expiry_time.ToDoubleT());

        creative_set.ad_conversions.push_back(ad_conversion);
      }

      // Creatives
      for (const auto& creative_node :
          creative_set_node["creatives"].GetArray()) {
        const std::string creative_instance_id =
            creative_node["creativeInstanceId"].GetString();

        // Type
        const auto type_node = creative_node["type"].GetObject();

        const std::string code = type_node["code"].GetString();
        if (code == "notification_all_v1") {
          CatalogAdNotificationCreativeInfo creative;

          creative.creative_instance_id = creative_instance_id;

          // Type
          creative.type.code = code;
          creative.type.name = type_node["name"].GetString();
          creative.type.platform = type_node["platform"].GetString();
          creative.type.version = type_node["version"].GetUint64();

          // Payload
          const auto payload_node = creative_node["payload"].GetObject();

          creative.payload.title = payload_node["title"].GetString();

          creative.payload.body = payload_node["body"].GetString();

          creative.payload.target_url = payload_node["targetUrl"].GetString();
          if (!GURL(creative.payload.target_url).is_valid()) {
            BLOG(1, "Invalid target URL for creative instance id "
                << creative_instance_id);
            continue;
          }

          creative_set.ad_notification_creatives.push_back(creative);
        } else if (code == "in_page_all_v1") {
          CatalogPublisherAdCreativeInfo creative;

          creative.creative_instance_id = creative_instance_id;

          // Type
          creative.type.code = code;
          creative.type.name = type_node["name"].GetString();
          creative.type.platform = type_node["platform"].GetString();
          creative.type.version = type_node["version"].GetUint64();

          // Payload
          const auto payload_node = creative_node["payload"].GetObject();

          creative.payload.creative_url =
              payload_node["creativeUrl"].GetString();
          if (!GURL(creative.payload.creative_url).is_valid()) {
            BLOG(1, "Invalid creative URL for creative instance id "
                << creative_instance_id);
            continue;
          }

          creative.payload.size = payload_node["size"].GetString();

          creative.payload.target_url = payload_node["targetUrl"].GetString();
          if (!GURL(creative.payload.target_url).is_valid()) {
            BLOG(1, "Invalid target URL for creative instance id "
                << creative_instance_id);
            continue;
          }

          // Channels
          for (const auto& channel_node :
              creative_set_node["channels"].GetArray()) {
            CatalogPublisherAdChannelInfo channel;
            channel.name = channel_node.GetString();

            creative.channels.push_back(channel);
          }

          creative_set.publisher_ad_creatives.push_back(creative);
        } else {
          // Unknown type
          NOTREACHED();
          continue;
        }
      }

      campaign.creative_sets.push_back(creative_set);
    }

    new_campaigns.push_back(campaign);
  }

  // Issuers
  for (const auto& issuer_node : document["issuers"].GetArray()) {
    CatalogIssuerInfo issuer;

    std::string name = issuer_node["name"].GetString();
    std::string public_key = issuer_node["publicKey"].GetString();

    if (name == "confirmation") {
      new_issuers.public_key = public_key;
      continue;
    }

    issuer.name = name;
    issuer.public_key = public_key;

    new_issuers.issuers.push_back(issuer);
  }

  catalog_id = new_catalog_id;
  version = new_version;
  ping = new_ping;
  campaigns = new_campaigns;
  issuers = new_issuers;

  return SUCCESS;
}

}  // namespace ads
