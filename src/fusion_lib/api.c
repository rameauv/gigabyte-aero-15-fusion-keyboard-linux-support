#include "api.h"

#include "commands.h"
#include "keyboard/keyboard.h"

#include <libusb.h>
#include <stdckdint.h>
#include <stdint.h>

bool isBrightnessValueValid(uint8_t value);
bool isSpeedValueValid(uint8_t value);
bool isModeColorValid(ModeColor color);
bool isModeValid(Mode mode);

[[nodiscard]] ApiStatus apiInit(Api *const pApi) {
  if (pApi == nullptr) {
    return API_RESULT_INVALID_ARGUMENT;
  }
  if (pApi->init) {
    return API_RESULT_OK;
  }
  pApi->init = false;
  if (keyboardInit(&pApi->keyboardData) != KEYBOARD_RESULT_OK) {
    return API_RESULT_INIT_FAILED;
  }
  pApi->init = true;
  return API_RESULT_OK;
}

[[nodiscard]] ApiStatus apiUninit(Api *const pApi) {
  if (pApi == nullptr) {
    return API_RESULT_INVALID_ARGUMENT;
  }
  if (!pApi->init) {
    return API_RESULT_NOT_INITIALIZED;
  }
  if (keyboardUnInit(&pApi->keyboardData) != KEYBOARD_RESULT_OK) {
    return API_RESULT_UNINIT_FAILED;
  }
  pApi->init = false;
  return API_RESULT_OK;
}

[[nodiscard]] ApiStatus apiGetCurrMode(const Api *const pApi, ModeData *const pOutMode) {
  if (pOutMode == nullptr || pApi == nullptr) {
    return API_RESULT_INVALID_ARGUMENT;
  }
  if (!pApi->init) {
    return API_RESULT_NOT_INITIALIZED;
  }
  if (pApi->keyboardData.dev_handle == nullptr) {
    return API_RESULT_GET_MODE_FAILED;
  }
  if (cmdGetCurrMode(pApi->keyboardData.dev_handle, pOutMode) != CMD_GET_CURR_MODE_RESULT_OK) {
    return API_RESULT_GET_MODE_FAILED;
  }
  return API_RESULT_OK;
}

[[nodiscard]] ApiStatus apiSetMode(const ModeData *const pMode, const Api *const pApi) {
  if (pMode == nullptr || pApi == nullptr) {
    return API_RESULT_INVALID_ARGUMENT;
  }
  if (!pApi->init) {
    return API_RESULT_NOT_INITIALIZED;
  }
  if (!isBrightnessValueValid(pMode->brightness)) {
    return API_RESULT_INVALID_ARGUMENT;
  }
  if (!isSpeedValueValid(pMode->speed)) {
    return API_RESULT_INVALID_ARGUMENT;
  }
  if (!isModeColorValid(pMode->color)) {
    return API_RESULT_INVALID_ARGUMENT;
  }
  if (!isModeValid(pMode->mode)) {
    return API_RESULT_INVALID_ARGUMENT;
  }
  if (pApi->keyboardData.dev_handle == nullptr) {
    return API_RESULT_SET_MODE_FAILED;
  }
  if (cmdSetMode(pApi->keyboardData.dev_handle, pMode) != CMD_SET_MODE_RESULT_OK) {
    return API_RESULT_SET_MODE_FAILED;
  }
  return API_RESULT_OK;
}

bool isBrightnessValueValid(uint8_t value) {
  uint8_t result = 0;
  if (ckd_sub(&result, value, FUSION_BRIGHTNESS_MIN)) {
    return false;
  }
  return result <= (FUSION_BRIGHTNESS_MAX - FUSION_BRIGHTNESS_MIN);
}

bool isSpeedValueValid(uint8_t value) {
  uint8_t result = 0;
  if (ckd_sub(&result, value, FUSION_SPEED_MIN)) {
    return false;
  }
  return result <= (FUSION_SPEED_MAX - FUSION_SPEED_MIN);
}

bool isModeColorValid(ModeColor color) { return (color >= MODE_COLOR_RED && color <= MODE_COLOR_WHITE) != 0; }

bool isModeValid(Mode mode) { return (mode >= MODE_STATIC && mode <= MODE_ROTATE) != 0; }
