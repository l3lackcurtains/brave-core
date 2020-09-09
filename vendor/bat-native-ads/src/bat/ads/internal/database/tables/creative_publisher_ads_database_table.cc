// /* Copyright (c) 2020 The Brave Authors. All rights reserved.
//  * This Source Code Form is subject to the terms of the Mozilla Public
//  * License, v. 2.0. If a copy of the MPL was not distributed with this file,
//  * You can obtain one at http://mozilla.org/MPL/2.0/. */

// #include "bat/ads/internal/database/tables/creative_publisher_ads_database_table.h"

// #include <functional>
// #include <set>
// #include <utility>

// #include "base/strings/string_util.h"
// #include "base/strings/stringprintf.h"
// #include "bat/ads/internal/ads_impl.h"
// #include "bat/ads/internal/container_util.h"
// #include "bat/ads/internal/database/database_statement_util.h"
// #include "bat/ads/internal/database/database_table_util.h"
// #include "bat/ads/internal/database/database_util.h"
// #include "bat/ads/internal/logging.h"
// #include "bat/ads/internal/time_util.h"

// namespace ads {
// namespace database {
// namespace table {

// using std::placeholders::_1;

// namespace {

// const char kTableName[] = "creative_publisher_ads";

// const int kDefaultBatchSize = 50;

// }  // namespace

// CreativePublisherAds::CreativePublisherAds(
//     AdsImpl* ads)
//     : batch_size_(kDefaultBatchSize),
//       ads_(ads),
//       geo_targets_database_table_(std::make_unique<GeoTargets>(ads_)),
//       categories_database_table_(std::make_unique<Categories>(ads_)) {
//   DCHECK(ads_);
// }

// CreativePublisherAds::~CreativePublisherAds() = default;

// void CreativePublisherAds::Save(
//     const CreativePublisherAdList& creative_publisher_ads,
//     ResultCallback callback) {
//   if (creative_publisher_ads.empty()) {
//     callback(Result::SUCCESS);
//     return;
//   }

//   DBTransactionPtr transaction = DBTransaction::New();

//   // TODO(https://github.com/brave/brave-browser/issues/3661) instead of
//   // rebuilding the catalog database each time
//   DeleteAllTables(transaction.get());

//   const std::vector<CreativePublisherAdList> batches =
//       SplitVector(creative_publisher_ads, batch_size_);

//   for (const auto& batch : batches) {
//     InsertOrUpdate(transaction.get(), batch);
//     geo_targets_database_table_->InsertOrUpdate(transaction.get(), batch);
//     categories_database_table_->InsertOrUpdate(transaction.get(), batch);
//   }

//   ads_->get_ads_client()->RunDBTransaction(std::move(transaction),
//       std::bind(&OnResultCallback, _1, callback));
// }

// void CreativePublisherAds::GetForCategories(
//     const classification::CategoryList& categories,
//     GetCreativePublisherAdsCallback callback) {
//   if (categories.empty()) {
//     callback(Result::SUCCESS, categories, {});
//     return;
//   }

//   const std::string query = base::StringPrintf(
//       "SELECT "
//           "can.creative_instance_id, "
//           "can.creative_set_id, "
//           "can.campaign_id, "
//           "can.start_at_timestamp, "
//           "can.end_at_timestamp, "
//           "can.daily_cap, "
//           "can.advertiser_id, "
//           "can.priority, "
//           "can.conversion, "
//           "can.per_day, "
//           "can.total_max, "
//           "c.category, "
//           "gt.geo_target, "
//           "can.creative_url, "
//           "can.size, "
//           "can.target_url, "
//           "can.ptr "
//       "FROM %s AS can "
//           "INNER JOIN categories AS c "
//               "ON c.creative_instance_id = can.creative_instance_id "
//           "INNER JOIN geo_targets AS gt "
//               "ON gt.creative_instance_id = can.creative_instance_id "
//       "WHERE c.category IN %s "
//           "AND %s BETWEEN can.start_at_timestamp AND can.end_at_timestamp",
//       get_table_name().c_str(),
//       BuildBindingParameterPlaceholder(categories.size()).c_str(),
//       NowAsString().c_str());

//   DBCommandPtr command = DBCommand::New();
//   command->type = DBCommand::Type::READ;
//   command->command = query;

//   int index = 0;
//   for (const auto& category : categories) {
//     BindString(command.get(), index, base::ToLowerASCII(category));
//     index++;
//   }

//   command->record_bindings = {
//     DBCommand::RecordBindingType::STRING_TYPE,  // creative_instance_id
//     DBCommand::RecordBindingType::STRING_TYPE,  // creative_set_id
//     DBCommand::RecordBindingType::STRING_TYPE,  // campaign_id
//     DBCommand::RecordBindingType::INT64_TYPE,   // start_at_timestamp
//     DBCommand::RecordBindingType::INT64_TYPE,   // end_at_timestamp
//     DBCommand::RecordBindingType::INT_TYPE,     // daily_cap
//     DBCommand::RecordBindingType::STRING_TYPE,  // advertiser_id
//     DBCommand::RecordBindingType::INT_TYPE,     // priority
//     DBCommand::RecordBindingType::BOOL_TYPE,    // conversion
//     DBCommand::RecordBindingType::INT_TYPE,     // per_day
//     DBCommand::RecordBindingType::INT_TYPE,     // total_max
//     DBCommand::RecordBindingType::STRING_TYPE,  // category
//     DBCommand::RecordBindingType::STRING_TYPE,  // geo_target
//     DBCommand::RecordBindingType::STRING_TYPE,  // target_url
//     DBCommand::RecordBindingType::STRING_TYPE,  // title
//     DBCommand::RecordBindingType::STRING_TYPE,  // body
//     DBCommand::RecordBindingType::DOUBLE_TYPE   // ptr
//   };

//   DBTransactionPtr transaction = DBTransaction::New();
//   transaction->commands.push_back(std::move(command));

//   ads_->get_ads_client()->RunDBTransaction(std::move(transaction),
//       std::bind(&CreativePublisherAds::OnGetCreativePublisherAds, this,
//           _1, categories, callback));
// }

// void CreativePublisherAds::GetAll(
//     GetCreativePublisherAdsCallback callback) {
//   const std::string query = base::StringPrintf(
//       "SELECT "
//           "can.creative_instance_id, "
//           "can.creative_set_id, "
//           "can.campaign_id, "
//           "can.start_at_timestamp, "
//           "can.end_at_timestamp, "
//           "can.daily_cap, "
//           "can.advertiser_id, "
//           "can.priority, "
//           "can.conversion, "
//           "can.per_day, "
//           "can.total_max, "
//           "c.category, "
//           "gt.geo_target, "
//           "can.creative_url, "
//           "can.size, "
//           "can.target_url, "
//           "can.ptr "
//       "FROM %s AS can "
//           "INNER JOIN categories AS c "
//               "ON c.creative_instance_id = can.creative_instance_id "
//           "INNER JOIN geo_targets AS gt "
//               "ON gt.creative_instance_id = can.creative_instance_id "
//       "WHERE %s BETWEEN can.start_at_timestamp AND can.end_at_timestamp",
//       get_table_name().c_str(),
//       NowAsString().c_str());

//   DBCommandPtr command = DBCommand::New();
//   command->type = DBCommand::Type::READ;
//   command->command = query;

//   command->record_bindings = {
//     DBCommand::RecordBindingType::STRING_TYPE,  // creative_instance_id
//     DBCommand::RecordBindingType::STRING_TYPE,  // creative_set_id
//     DBCommand::RecordBindingType::STRING_TYPE,  // campaign_id
//     DBCommand::RecordBindingType::INT64_TYPE,   // start_at_timestamp
//     DBCommand::RecordBindingType::INT64_TYPE,   // end_at_timestamp
//     DBCommand::RecordBindingType::INT_TYPE,     // daily_cap
//     DBCommand::RecordBindingType::STRING_TYPE,  // advertiser_id
//     DBCommand::RecordBindingType::INT_TYPE,     // priority
//     DBCommand::RecordBindingType::BOOL_TYPE,    // conversion
//     DBCommand::RecordBindingType::INT_TYPE,     // per_day
//     DBCommand::RecordBindingType::INT_TYPE,     // total_max
//     DBCommand::RecordBindingType::STRING_TYPE,  // category
//     DBCommand::RecordBindingType::STRING_TYPE,  // geo_target
//     DBCommand::RecordBindingType::STRING_TYPE,  // target_url
//     DBCommand::RecordBindingType::STRING_TYPE,  // title
//     DBCommand::RecordBindingType::STRING_TYPE,  // body
//     DBCommand::RecordBindingType::DOUBLE_TYPE   // ptr
//   };

//   DBTransactionPtr transaction = DBTransaction::New();
//   transaction->commands.push_back(std::move(command));

//   ads_->get_ads_client()->RunDBTransaction(std::move(transaction),
//       std::bind(&CreativePublisherAds::OnGetAllCreativePublisherAds,
//           this, _1, callback));
// }

// void CreativePublisherAds::set_batch_size(
//     const int batch_size) {
//   DCHECK_GT(batch_size, 0);

//   batch_size_ = batch_size;
// }

// std::string CreativePublisherAds::get_table_name() const {
//   return kTableName;
// }

// void CreativePublisherAds::Migrate(
//     DBTransaction* transaction,
//     const int to_version) {
//   DCHECK(transaction);

//   switch (to_version) {
//     case 1: {
//       MigrateToV1(transaction);
//       break;
//     }

//     case 2: {
//       MigrateToV2(transaction);
//       break;
//     }

//     default: {
//       break;
//     }
//   }
// }

// ///////////////////////////////////////////////////////////////////////////////

// void CreativePublisherAds::InsertOrUpdate(
//     DBTransaction* transaction,
//     const CreativePublisherAdList& creative_publisher_ads) {
//   DCHECK(transaction);

//   if (creative_publisher_ads.empty()) {
//     return;
//   }

//   DBCommandPtr command = DBCommand::New();
//   command->type = DBCommand::Type::RUN;
//   command->command = BuildInsertOrUpdateQuery(command.get(),
//       creative_publisher_ads);

//   transaction->commands.push_back(std::move(command));
// }

// int CreativePublisherAds::BindParameters(
//     DBCommand* command,
//     const CreativePublisherAdList& creative_publisher_ads) {
//   DCHECK(command);

//   int count = 0;

//   int index = 0;
//   for (const auto& creative_publisher_ad : creative_publisher_ads) {
//     BindString(command, index++, creative_publisher_ad.creative_instance_id);
//     BindString(command, index++, creative_publisher_ad.creative_set_id);
//     BindString(command, index++, creative_publisher_ad.campaign_id);
//     BindInt64(command, index++, creative_publisher_ad.start_at_timestamp);
//     BindInt64(command, index++, creative_publisher_ad.end_at_timestamp);
//     BindInt64(command, index++, creative_publisher_ad.daily_cap);
//     BindString(command, index++, creative_publisher_ad.advertiser_id);
//     BindInt64(command, index++, creative_publisher_ad.priority);
//     BindBool(command, index++, creative_publisher_ad.conversion);
//     BindInt64(command, index++, creative_publisher_ad.per_day);
//     BindInt64(command, index++, creative_publisher_ad.total_max);
//     BindString(command, index++, creative_publisher_ad.creative_url);
//     BindString(command, index++, creative_publisher_ad.size);
//     BindString(command, index++, creative_publisher_ad.target_url);
//     BindDouble(command, index++, creative_publisher_ad.ptr);

//     count++;
//   }

//   return count;
// }

// std::string CreativePublisherAds::BuildInsertOrUpdateQuery(
//     DBCommand* command,
//     const CreativePublisherAdList& creative_publisher_ads) {
//   const int count = BindParameters(command, creative_publisher_ads);

//   return base::StringPrintf(
//       "INSERT OR REPLACE INTO %s "
//           "(creative_instance_id, "
//           "creative_set_id, "
//           "campaign_id, "
//           "start_at_timestamp, "
//           "end_at_timestamp, "
//           "daily_cap, "
//           "advertiser_id, "
//           "priority, "
//           "conversion, "
//           "per_day, "
//           "total_max, "
//           "target_url, "
//           "title, "
//           "body, "
//           "ptr) VALUES %s",
//       get_table_name().c_str(),
//       BuildBindingParameterPlaceholders(15, count).c_str());
// }

// void CreativePublisherAds::OnGetCreativePublisherAds(
//     DBCommandResponsePtr response,
//     const classification::CategoryList& categories,
//     GetCreativePublisherAdsCallback callback) {
//   if (!response || response->status != DBCommandResponse::Status::RESPONSE_OK) {
//     BLOG(0, "Failed to get creative ad notifications");
//     callback(Result::FAILED, categories, {});
//     return;
//   }

//   CreativePublisherAdList creatives;

//   for (auto const& record : response->result->get_records()) {
//     const CreativePublisherAdInfo creative =
//         GetCreativePublisherAdFromRecord(record.get());

//     creatives.emplace_back(creative);
//   }

//   callback(Result::SUCCESS, categories, creatives);
// }

// void CreativePublisherAds::OnGetAllCreativePublisherAds(
//     DBCommandResponsePtr response,
//     GetCreativePublisherAdsCallback callback) {
//   if (!response || response->status != DBCommandResponse::Status::RESPONSE_OK) {
//     BLOG(0, "Failed to get all creative ad notifications");
//     callback(Result::FAILED, {}, {});
//     return;
//   }

//   CreativeAdNotificationList creatives;

//   classification::CategoryList categories;

//   for (auto const& record : response->result->get_records()) {
//     const CreativeAdNotificationInfo creative =
//         GetCreativeAdNotificationFromRecord(record.get());

//     creatives.push_back(creative);

//     categories.push_back(creative.category);
//   }

//   callback(Result::SUCCESS, categories, creatives);
// }

// CreativePublisherAdInfo
// CreativePublisherAds::GetCreativePublisherAdFromRecord(
//     DBRecord* record) const {
//   CreativePublisherAdInfo creative;

//   creative.creative_instance_id = ColumnString(record, 0);
//   creative.creative_set_id = ColumnString(record, 1);
//   creative.campaign_id = ColumnString(record, 2);
//   creative.start_at_timestamp = ColumnInt64(record, 3);
//   creative.end_at_timestamp = ColumnInt64(record, 4);
//   creative.daily_cap = ColumnInt(record, 5);
//   creative.advertiser_id = ColumnString(record, 6);
//   creative.priority = ColumnInt(record, 7);
//   creative.conversion = ColumnBool(record, 8);
//   creative.per_day = ColumnInt(record, 9);
//   creative.total_max = ColumnInt(record, 10);
//   creative.category = ColumnString(record, 11);
//   creative.geo_targets.push_back(ColumnString(record, 12));
//   creative.creative_url = ColumnString(record, 13);
//   creative.size = ColumnString(record, 14);
//   creative.target_url = ColumnString(record, 15);
//   creative.ptr = ColumnDouble(record, 16);

//   return creative;
// }

// void CreativePublisherAds::DeleteAllTables(
//     DBTransaction* transaction) const {
//   Delete(transaction, get_table_name());
//   Delete(transaction, geo_targets_database_table_->get_table_name());
//   Delete(transaction, categories_database_table_->get_table_name());
// }

// void CreativePublisherAds::CreateTableV1(
//     DBTransaction* transaction) {
//   DCHECK(transaction);

//   const std::string query = base::StringPrintf(
//       "CREATE TABLE %s "
//           "(creative_instance_id TEXT NOT NULL, "
//           "creative_set_id TEXT NOT NULL, "
//           "campaign_id TEXT NOT NULL, "
//           "start_at_timestamp TIMESTAMP NOT NULL, "
//           "end_at_timestamp TIMESTAMP NOT NULL, "
//           "daily_cap INTEGER DEFAULT 0 NOT NULL, "
//           "advertiser_id LONGVARCHAR, "
//           "priority INTEGER NOT NULL DEFAULT 0, "
//           "conversion INTEGER NOT NULL DEFAULT 0, "
//           "per_day INTEGER NOT NULL DEFAULT 0, "
//           "total_max INTEGER NOT NULL DEFAULT 0, "
//           "target_url TEXT NOT NULL, "
//           "title TEXT NOT NULL, "
//           "body TEXT NOT NULL, "
//           "PRIMARY KEY(creative_instance_id))",
//       get_table_name().c_str());

//   DBCommandPtr command = DBCommand::New();
//   command->type = DBCommand::Type::EXECUTE;
//   command->command = query;

//   transaction->commands.push_back(std::move(command));
// }

// void CreativePublisherAds::MigrateToV1(
//     DBTransaction* transaction) {
//   DCHECK(transaction);

//   Drop(transaction, get_table_name());

//   CreateTableV1(transaction);
// }

// void CreativePublisherAds::MigrateToV2(
//     DBTransaction* transaction) {
//   DCHECK(transaction);

//   const std::string query = base::StringPrintf(
//       "ALTER TABLE %s "
//           "ADD ptr DOUBLE NOT NULL DEFAULT 1",
//       get_table_name().c_str());

//   DBCommandPtr command = DBCommand::New();
//   command->type = DBCommand::Type::EXECUTE;
//   command->command = query;

//   transaction->commands.push_back(std::move(command));
// }

// }  // namespace table
// }  // namespace database
// }  // namespace ads
