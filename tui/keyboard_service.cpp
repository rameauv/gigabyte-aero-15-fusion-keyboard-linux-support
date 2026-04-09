#include "tui/keyboard_service.h"

#include "fusion_lib/commands.h"

KeyboardService::KeyboardService() {
  if (apiInit(&api_) != API_OK) {
    isReady_   = false;
    lastError_ = "Failed to initialize API";
    return;
  }
  isReady_ = true;
}

KeyboardService::~KeyboardService() {
  if (isReady_) {
    apiUninit(&api_);
  }
}

bool KeyboardService::isReady() const { return isReady_; }

const std::string &KeyboardService::lastError() const { return lastError_; }

bool KeyboardService::getCurrentMode(ModeData &outMode) {
  if (!isReady_) {
    lastError_ = "API not ready";
    return false;
  }
  if (apiGetCurrMode(&api_, &outMode) != API_OK) {
    lastError_ = "Failed to read current mode from device";
    return false;
  }
  return true;
}

bool KeyboardService::setMode(const ModeData &modeData) {
  if (!isReady_) {
    lastError_ = "API not ready";
    return false;
  }
  ModeData writable = modeData;
  if (apiSetMode(&writable, &api_) != API_OK) {
    lastError_ = "Failed to set mode on device";
    return false;
  }
  return true;
}
