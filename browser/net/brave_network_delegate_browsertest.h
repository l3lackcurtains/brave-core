/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_NET_BRAVE_NETWORK_DELEGATE_BROWSERTEST_H_
#define BRAVE_BROWSER_NET_BRAVE_NETWORK_DELEGATE_BROWSERTEST_H_

#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "url/gurl.h"

class BraveNetworkDelegateBrowserTest : public InProcessBrowserTest {
 public:
  BraveNetworkDelegateBrowserTest();
  ~BraveNetworkDelegateBrowserTest() override;

  void SetUpOnMainThread() override;
  HostContentSettingsMap* content_settings();
  void SetUpCommandLine(base::CommandLine* command_line) override;

  void DefaultBlockAllCookies();
  void DefaultBlockThirdPartyCookies();
  void DefaultAllowAllCookies();

  void AllowCookies(const GURL url);
  void BlockThirdPartyCookies(const GURL url);
  void BlockCookies(const GURL url);
  void ShieldsDown(const GURL url);

  void NavigateToPageWithFrame(const GURL url);
  void NavigateFrameTo(const GURL url, const std::string& id = "test");

  void ExpectCookiesOnHost(const GURL url,
                           const std::string& expected);
  void BlockGoogleOAuthCookies();

 protected:
  GURL url_;
  GURL nested_iframe_script_url_;
  GURL top_level_page_url_;
  GURL https_top_level_page_url_;
  GURL cookie_iframe_url_;
  GURL https_cookie_iframe_url_;
  GURL third_party_cookie_url_;
  GURL first_party_cookie_url_;
  GURL subdomain_first_party_cookie_url_;
  GURL domain_registry_url_;
  GURL iframe_domain_registry_url_;
  GURL google_oauth_cookie_url_;
  GURL wordpress_top_url_;
  GURL wordpress_frame_url_;
  GURL wp_top_url_;
  GURL wp_frame_url_;
  GURL a_frame_url_;
  net::test_server::EmbeddedTestServer https_server_;

 private:
  ContentSettingsPattern top_level_page_pattern_;
  ContentSettingsPattern first_party_pattern_;
  ContentSettingsPattern iframe_pattern_;

  DISALLOW_COPY_AND_ASSIGN(BraveNetworkDelegateBrowserTest);
};

#endif  // BRAVE_BROWSER_NET_BRAVE_NETWORK_DELEGATE_BROWSERTEST_H_

