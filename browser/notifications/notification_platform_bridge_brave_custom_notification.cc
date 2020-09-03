/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/notifications/notification_platform_bridge_brave_custom_notification.h"

#include "base/bind.h"
#include "base/callback.h"
#include "base/memory/scoped_refptr.h"
#include "base/run_loop.h"
#include "base/task/post_task.h"
#include "brave/components/brave_ads/browser/ads_notification_handler.h"
#include "brave/components/brave_ads/browser/ads_service.h"
#include "brave/components/brave_ads/browser/ads_service_factory.h"
#include "brave/components/brave_ads/browser/ads_service_impl.h"
#include "brave/ui/brave_custom_notification/message_popup_view.h"
#include "brave/ui/brave_custom_notification/public/cpp/notification.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"

namespace {

class PassThroughDelegate
    : public brave_custom_notification::NotificationDelegate {
 public:
  PassThroughDelegate(
      Profile* profile,
      const brave_custom_notification::Notification& notification)
      : profile_(profile),
        notification_(notification) {}

  void Close(bool by_user) override {
    std::unique_ptr<brave_ads::AdsNotificationHandler> handler =
        std::make_unique<brave_ads::AdsNotificationHandler>(
            static_cast<content::BrowserContext*>(profile_));
    handler->SetAdsService(static_cast<brave_ads::AdsServiceImpl*>(brave_ads::AdsServiceFactory::GetForProfile(profile_)));
    handler->OnClose(profile_, notification_.origin_url(), notification_.id(), by_user, base::OnceClosure());
  }

  void Click(const base::Optional<int>& button_index,
             const base::Optional<base::string16>& reply) override {
    std::unique_ptr<brave_ads::AdsNotificationHandler> handler = std::make_unique<brave_ads::AdsNotificationHandler>(static_cast<content::BrowserContext*>(profile_));
    handler->SetAdsService(static_cast<brave_ads::AdsServiceImpl*>(brave_ads::AdsServiceFactory::GetForProfile(profile_)));
    handler->OnClick(profile_, notification_.origin_url(), notification_.id(), button_index, reply, base::OnceClosure());
  }

 protected:
  ~PassThroughDelegate() override = default;

 private:
  Profile* profile_;
  brave_custom_notification::Notification notification_;

  DISALLOW_COPY_AND_ASSIGN(PassThroughDelegate);
};

}  // namespace

NotificationPlatformBridgeBraveCustomNotification::
    NotificationPlatformBridgeBraveCustomNotification(Profile* profile)
    : profile_(profile) {}

NotificationPlatformBridgeBraveCustomNotification::
    ~NotificationPlatformBridgeBraveCustomNotification() = default;

void NotificationPlatformBridgeBraveCustomNotification::Display(
    Profile* profile,
    const brave_custom_notification::Notification& notification) {
  DCHECK_EQ(profile, profile_);

  // If there's no delegate, replace it with a PassThroughDelegate so clicks
  // go back to the appropriate handler.
  notification.set_delegate(base::WrapRefCounted(
      new PassThroughDelegate(profile_, notification)));

  brave_custom_notification::MessagePopupView::Show(notification);

  std::unique_ptr<brave_ads::AdsNotificationHandler> handler = std::make_unique<brave_ads::AdsNotificationHandler>(static_cast<content::BrowserContext*>(profile));
  handler->OnShow(profile_, notification.id());
}

void NotificationPlatformBridgeBraveCustomNotification::Close(
    Profile* profile,
    const std::string& notification_id) {
    brave_custom_notification::MessagePopupView::ClosePopup();
}

