
#include "brave/vendor/brave-ios/components/brave_sync/brave_sync_worker.h"
#include "brave/ios/browser/browser_state/browser_state_manager.h"
#include "brave/vendor/brave-ios/components/brave_sync/brave_sync_service.h"

#include <string>
#include <vector>

#include "base/strings/sys_string_conversions.h"
#include "base/json/json_writer.h"
#include "base/path_service.h"
#include "base/strings/string_number_conversions.h"

#include "brave/components/brave_sync/brave_sync_prefs.h"
#include "brave/components/brave_sync/crypto/crypto.h"

//#include "chrome/browser/sync/device_info_sync_service_factory.h"
//#include "chrome/browser/sync/profile_sync_service_factory.h"

#include "components/sync/driver/sync_service.h"
#include "components/sync/driver/sync_user_settings.h"
#include "components/sync_device_info/device_info.h"
#include "components/sync_device_info/device_info_sync_service.h"
#include "components/sync_device_info/device_info_tracker.h"
#include "components/sync_device_info/local_device_info_provider.h"

#include "ios/chrome/browser/browser_state/chrome_browser_state.h"
#include "ios/chrome/browser/sync/profile_sync_service_factory.h"
#include "ios/chrome/browser/sync/device_info_sync_service_factory.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

//namespace {
//static const size_t SEED_BYTES_COUNT = 32u;
//}  // namespace

@interface BraveSyncWorker()
{
    ChromeBrowserState* browser_state_;
}
@end

@implementation BraveSyncWorker
- (instancetype)init {
    if ((self = [super init])) {
        browser_state_ = BrowserStateManager::GetInstance().GetBrowserState();
        CHECK(browser_state_);
    }
    return self;
}

- (void)dealloc {
    browser_state_ = nullptr;
}

- (NSString *)getSyncCodeWords {
    brave_sync::Prefs brave_sync_prefs(browser_state_->GetPrefs());
    std::string sync_code = brave_sync_prefs.GetSeed();

    if (sync_code.empty()) {
      std::vector<uint8_t> seed = brave_sync::crypto::GetSeed();
      sync_code = brave_sync::crypto::PassphraseFromBytes32(seed);
      brave_sync_prefs.SetSeed(sync_code);
        
      VLOG(3) << "[BraveSync] " << __PRETTY_FUNCTION__ << " generated new sync code";
    }
    return base::SysUTF8ToNSString(sync_code.c_str());
}

- (bool)setSyncCodeWords:(NSString *)passphrase {
    std::vector<uint8_t> seed;
    std::string code_words = base::SysNSStringToUTF8(passphrase);
    
    if (!brave_sync::crypto::PassphraseToBytes32(code_words, &seed)) {
      LOG(ERROR) << "[BraveSync] Invalid sync code: " << code_words;
      return false;
    }
    
    brave_sync::Prefs brave_sync_prefs{browser_state_->GetPrefs()};
    brave_sync_prefs.SetSeed(code_words);
    return true;
}

- (syncer::SyncService *)getSyncService {
    return static_cast<syncer::SyncService*>(
                                             ProfileSyncServiceFactory::GetForBrowserState(browser_state_));
}

- (syncer::DeviceInfoTracker *)getDeviceInfoTracker {
  auto* device_info_sync_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);
  return device_info_sync_service->GetDeviceInfoTracker();
}

- (syncer::LocalDeviceInfoProvider *)getLocalDeviceInfoProvider {
  auto* device_info_sync_service =
      DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);
  return device_info_sync_service->GetLocalDeviceInfoProvider();
}

- (std::vector<std::unique_ptr<DeviceInfo>>)getDeviceList {
    std::vector<std::unique_ptr<syncer::DeviceInfo>> result;
    
    auto* device_info_service =
        DeviceInfoSyncServiceFactory::GetForBrowserState(browser_state_);
    syncer::DeviceInfoTracker* tracker =
        device_info_service->GetDeviceInfoTracker();
    DCHECK(tracker);
    
    const syncer::DeviceInfo* local_device_info = device_info_service
       ->GetLocalDeviceInfoProvider()->GetLocalDeviceInfo();
    
//    const std::vector<std::unique_ptr<syncer::DeviceInfo>> all_devices =
//    device_sync_service->GetDeviceInfoTracker()->GetAllDeviceInfo();
    
    for (const auto& device : tracker->GetAllDeviceInfo()) {
      bool is_current_device =
          local_device_info ? local_device_info->guid() == device->guid() : false;
        
        
      //device_value.SetBoolKey("isCurrentDevice", is_current_device);
      //device_list.Append(std::move(device_value));
    }
}
@end
