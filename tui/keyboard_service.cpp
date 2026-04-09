#include "tui/keyboard_service.h"

#include "fusion_lib/api.h"
#include "fusion_lib/commands.h"

const std::string &KeyboardService::lastError() const { return lastError_; }

bool KeyboardService::getCurrentMode(ModeData &outMode) {
  Api api{};
  if (apiInit(&api) != API_OK) {
    lastError_ = "Failed to initialize API";
    return false;
  }
  if (apiGetCurrMode(&api, &outMode) != API_OK) {
    lastError_ = "Failed to read current mode from device";
    apiUninit(&api);
    return false;
  }
  apiUninit(&api);
  return true;
}

bool KeyboardService::setMode(const ModeData &modeData) {
  Api api{};
  if (apiInit(&api) != API_OK) {
    lastError_ = "Failed to initialize API";
    return false;
  }
  ModeData writable = modeData;
  if (apiSetMode(&writable, &api) != API_OK) {
    lastError_ = "Failed to set mode on device";
    apiUninit(&api);
    return false;
  }
  apiUninit(&api);
  return true;
}
