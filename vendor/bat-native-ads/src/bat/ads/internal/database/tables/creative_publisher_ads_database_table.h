// /* Copyright (c) 2020 The Brave Authors. All rights reserved.
//  * This Source Code Form is subject to the terms of the Mozilla Public
//  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
//  * You can obtain one at http://mozilla.org/MPL/2.0/. */

// #ifndef BAT_ADS_INTERNAL_DATABASE_CREATIVE_PUBLISHER_ADS_DATABASE_TABLE_H_
// #define BAT_ADS_INTERNAL_DATABASE_CREATIVE_PUBLISHER_ADS_DATABASE_TABLE_H_

// #include <memory>
// #include <string>
// #include <vector>

// #include "bat/ads/ads_client.h"
// #include "bat/ads/internal/classification/page_classifier/page_classifier.h"
// #include "bat/ads/internal/bundle/creative_publisher_ad_info.h"
// #include "bat/ads/internal/database/database_table.h"
// #include "bat/ads/internal/database/tables/categories_database_table.h"
// #include "bat/ads/internal/database/tables/geo_targets_database_table.h"
// #include "bat/ads/mojom.h"
// #include "bat/ads/result.h"

// namespace ads {

// using GetCreativePublisherAdsCallback = std::function<void(const Result,
//     const std::vector<std::string>&, const CreativePublisherAdList&)>;

// class AdsImpl;

// namespace database {
// namespace table {

// class CreativePublisherAds : public Table {
//  public:
//   explicit CreativePublisherAds(
//       AdsImpl* ads);

//   ~CreativePublisherAds() override;

//   void Save(
//       const CreativePublisherAdList& creative_publisher_ads,
//       ResultCallback callback);

//   void GetForCategories(
//       const classification::CategoryList& categories,
//       GetCreativePublisherAdsCallback callback);

//   void GetAll(
//       GetCreativePublisherAdsCallback callback);

//   void set_batch_size(
//       const int batch_size);

//   std::string get_table_name() const override;

//   void Migrate(
//       DBTransaction* transaction,
//       const int to_version) override;

//  private:
//   void InsertOrUpdate(
//       DBTransaction* transaction,
//       const CreativePublisherAdList& creative_publisher_ads);

//   int BindParameters(
//       DBCommand* command,
//       const CreativePublisherAdList& creative_publisher_ads);

//   std::string BuildInsertOrUpdateQuery(
//       DBCommand* command,
//       const CreativePublisherAdList& creative_publisher_ads);

//   void OnGetCreativePublisherAds(
//       DBCommandResponsePtr response,
//       const classification::CategoryList& categories,
//       GetCreativePublisherAdsCallback callback);

//   void OnGetAllCreativePublisherAds(
//       DBCommandResponsePtr response,
//       GetCreativePublisherAdsCallback callback);

//   CreativePublisherAdInfo GetCreativePublisherAdFromRecord(
//       DBRecord* record) const;

//   void DeleteAllTables(
//       DBTransaction* transaction) const;

//   void CreateTableV1(
//       DBTransaction* transaction);
//   void MigrateToV1(
//       DBTransaction* transaction);

//   void MigrateToV2(
//       DBTransaction* transaction);

//   int batch_size_;

//   AdsImpl* ads_;  // NOT OWNED

//   std::unique_ptr<GeoTargets> geo_targets_database_table_;
//   std::unique_ptr<Categories> categories_database_table_;
// };

// }  // namespace table
// }  // namespace database
// }  // namespace ads

// #endif  // BAT_ADS_INTERNAL_DATABASE_CREATIVE_PUBLISHER_ADS_DATABASE_TABLE_H_
