#pragma once

#include <libusb.h>

#define KEYBOARD_VID 0x1044
#define KEYBOARD_PID 0x7a39
#define KEYBOARD_IFACE3 3

typedef enum KeyboardResult {
  KEYBOARD_RESULT_OK = 0,
  KEYBOARD_RESULT_INVALID_ARGUMENT,
  KEYBOARD_RESULT_INIT_FAILED,
  KEYBOARD_RESULT_NOT_INITIALIZED,
  KEYBOARD_RESULT_DEVICE_OPEN_FAILED,
  KEYBOARD_RESULT_AUTO_DETACH_IFACE0_FAILED,
  KEYBOARD_RESULT_AUTO_DETACH_IFACE3_FAILED,
  KEYBOARD_RESULT_CLAIM_IFACE0_FAILED,
  KEYBOARD_RESULT_CLAIM_IFACE3_FAILED
} KeyboardResult;

typedef struct KeyboardData {
  libusb_context *ctx;
  libusb_device_handle *dev_handle;
} KeyboardData;

[[nodiscard]] KeyboardResult keyboardInit(KeyboardData *pKeyboardData);
[[nodiscard]] KeyboardResult keyboardUnInit(KeyboardData *pKeyboardData);
