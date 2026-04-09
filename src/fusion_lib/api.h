#pragma once

#include "keyboard/keyboard.h"

#include <libusb.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FUSION_SPEED_MIN 0
#define FUSION_SPEED_MAX 10

#define FUSION_BRIGHTNESS_MIN 0
#define FUSION_BRIGHTNESS_MAX 50

#define API_OK (0)

typedef enum ModeColor : uint8_t {
  MODE_COLOR_RED    = 0x01,
  MODE_COLOR_GREEN  = 0x02,
  MODE_COLOR_YELLOW = 0x03,
  MODE_COLOR_BLUE   = 0x04,
  MODE_COLOR_ORANGE = 0x05,
  MODE_COLOR_PURPLE = 0x06,
  MODE_COLOR_WHITE  = 0x07,
} ModeColor;

typedef enum Mode : uint8_t {
  MODE_STATIC            = 0x01,
  MODE_BREATHING         = 0x02,
  MODE_WAVE              = 0x03,
  MODE_FADE_ON_KEYPRESS  = 0x04,
  MODE_MARQUEE           = 0x05,
  MODE_RIPPLE            = 0x06,
  MODE_FLASH_ON_KEYPRESS = 0x07,
  MODE_NEON              = 0x08,
  MODE_RAINBOW_MARQUEE   = 0x09,
  MODE_RAINDROP          = 0x0a,
  MODE_CIRCLE_MARQUEE    = 0x0b,
  MODE_HEDGE             = 0x0c,
  MODE_ROTATE            = 0x0d,
} Mode;

typedef struct ModeData {
  Mode mode;
  uint8_t speed;      // between 0 and 10
  uint8_t brightness; // between 0 and 50
  ModeColor color;
} ModeData;

typedef struct Api {
  bool init;
  KeyboardData keyboardData;
} Api;

typedef enum ApiStatus {
  API_RESULT_OK = 0,
  API_RESULT_INVALID_ARGUMENT,
  API_RESULT_NOT_INITIALIZED,
  API_RESULT_INIT_FAILED,
  API_RESULT_GET_MODE_FAILED,
  API_RESULT_SET_MODE_FAILED,
  API_RESULT_UNINIT_FAILED
} ApiStatus;

/**
 * @param pApi Api struct to initialize
 * @return API_STATUS_OK on success
 */
[[nodiscard]] ApiStatus apiInit(Api *pApi);
/**
 * @param pApi Api struct to uninitialize
 * @return API_STATUS_OK on success
 */
[[nodiscard]] ApiStatus apiUninit(Api *pApi);
/**
 * @param pMode the output mode
 * @param pApi
 * @return API_STATUS_OK on success
 */
[[nodiscard]] ApiStatus apiGetCurrMode(const Api *pApi, ModeData *pOutMode);
/**
 * @param pMode the mode to set
 * @param pApi
 * @return API_STATUS_OK on success
 */
[[nodiscard]] ApiStatus apiSetMode(const ModeData *pMode, const Api *pApi);

#ifdef __cplusplus
}
#endif
