/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_NOTIFICATIONS_NOTIFICATION_PLATFORM_BRIDGE_BRAVE_CUSTOM_NOTIFICATION_H_
#define BRAVE_BROWSER_NOTIFICATIONS_NOTIFICATION_PLATFORM_BRIDGE_BRAVE_CUSTOM_NOTIFICATION_H_

#include "base/macros.h"
#include "brave/components/brave_ads/browser/ads_notification_handler.h"
#include "brave/ui/brave_custom_notification/public/cpp/notification.h"

class Profile;

class NotificationPlatformBridgeBraveCustomNotification {
 public:
  explicit NotificationPlatformBridgeBraveCustomNotification(Profile* profile);
  ~NotificationPlatformBridgeBraveCustomNotification();

  void Display(Profile* profile,
               const brave_custom_notification::Notification& notification);
  void Close(Profile* profile, const std::string& notification_id);

 private:
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(NotificationPlatformBridgeBraveCustomNotification);
};

#endif
