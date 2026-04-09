#include "src/tui/keyboard_service.h"

#include "src/fusion_lib/api.h"

auto KeyboardService::lastError() const -> const std::string & { return lastError_; }

namespace {
auto statusToString(ApiStatus status) -> std::string {
  switch (status) {
  case API_RESULT_OK:
    return "OK";
  case API_RESULT_INVALID_ARGUMENT:
    return "Invalid argument";
  case API_RESULT_NOT_INITIALIZED:
    return "API not initialized";
  case API_RESULT_INIT_FAILED:
    return "Initialization failed";
  case API_RESULT_GET_MODE_FAILED:
    return "Could not read mode from device";
  case API_RESULT_SET_MODE_FAILED:
    return "Could not apply mode to device";
  case API_RESULT_UNINIT_FAILED:
    return "Uninitialization failed";
  }
  return "Unknown API status";
}
} // namespace

[[nodiscard]] auto KeyboardService::getCurrentMode(ModeData &outMode) -> bool {
  Api api{};
  ApiStatus status = apiInit(&api);
  if (status != API_RESULT_OK) {
    lastError_ = "Failed to initialize API: " + statusToString(status);
    return false;
  }
  status = apiGetCurrMode(&api, &outMode);
  if (status != API_RESULT_OK) {
    lastError_ = "Failed to read current mode from device: " + statusToString(status);
    (void)apiUninit(&api);
    return false;
  }
  status = apiUninit(&api);
  if (status != API_RESULT_OK) {
    lastError_ = "Failed to uninitialize API: " + statusToString(status);
    return false;
  }
  return true;
}

[[nodiscard]] auto KeyboardService::setMode(const ModeData &modeData) -> bool {
  Api api{};
  ApiStatus status = apiInit(&api);
  if (status != API_RESULT_OK) {
    lastError_ = "Failed to initialize API: " + statusToString(status);
    return false;
  }
  ModeData writable = modeData;
  status            = apiSetMode(&writable, &api);
  if (status != API_RESULT_OK) {
    lastError_ = "Failed to set mode on device: " + statusToString(status);
    (void)apiUninit(&api);
    return false;
  }
  status = apiUninit(&api);
  if (status != API_RESULT_OK) {
    lastError_ = "Failed to uninitialize API: " + statusToString(status);
    return false;
  }
  return true;
}
