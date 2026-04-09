#include "tui/keyboard_service.h"

#include "fusion_lib/commands.h"

const std::string &KeyboardService::lastError() const { return lastError_; }

bool KeyboardService::getCurrentMode(ModeData &outMode) {
  if (apiInit(&api_) != API_OK) {
    lastError_ = "Failed to initialize API";
    return false;
  }
  if (apiGetCurrMode(&api_, &outMode) != API_OK) {
    lastError_ = "Failed to read current mode from device";
    apiUninit(&api_);
    return false;
  }
  apiUninit(&api_);
  return true;
}

bool KeyboardService::setMode(const ModeData &modeData) {
  if (apiInit(&api_) != API_OK) {
    lastError_ = "Failed to initialize API";
    return false;
  }
  ModeData writable = modeData;
  if (apiSetMode(&writable, &api_) != API_OK) {
    lastError_ = "Failed to set mode on device";
    apiUninit(&api_);
    return false;
  }
  apiUninit(&api_);
  return true;
}
