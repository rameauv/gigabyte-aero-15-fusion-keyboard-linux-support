#include "../keyboard.h"

[[nodiscard]] KeyboardResult initLibusb(KeyboardData *pKeyboardData);
[[nodiscard]] KeyboardResult initDevice(KeyboardData *pKeyboardData);

[[nodiscard]] KeyboardResult keyboardInit(KeyboardData *const pKeyboardData) {
  if (pKeyboardData == nullptr) {
    return KEYBOARD_RESULT_INVALID_ARGUMENT;
  }
  pKeyboardData->ctx        = nullptr;
  pKeyboardData->dev_handle = nullptr;

  KeyboardResult initLibusbRes = initLibusb(pKeyboardData);
  if (initLibusbRes != KEYBOARD_RESULT_OK) {
    return initLibusbRes;
  }

  KeyboardResult initDeviceRes = initDevice(pKeyboardData);
  if (initDeviceRes != KEYBOARD_RESULT_OK) {
    libusb_exit(pKeyboardData->ctx);
    pKeyboardData->ctx = nullptr;
    return initDeviceRes;
  }

  return KEYBOARD_RESULT_OK;
}

[[nodiscard]] KeyboardResult keyboardUnInit(KeyboardData *const pKeyboardData) {
  if (pKeyboardData == nullptr) {
    return KEYBOARD_RESULT_INVALID_ARGUMENT;
  }
  if (pKeyboardData->ctx == nullptr || pKeyboardData->dev_handle == nullptr) {
    return KEYBOARD_RESULT_NOT_INITIALIZED;
  }

  (void)libusb_release_interface(pKeyboardData->dev_handle, KEYBOARD_IFACE3);
  libusb_close(pKeyboardData->dev_handle);
  libusb_exit(pKeyboardData->ctx);

  pKeyboardData->ctx        = nullptr;
  pKeyboardData->dev_handle = nullptr;

  return KEYBOARD_RESULT_OK;
}

[[nodiscard]] KeyboardResult initDevice(KeyboardData *pKeyboardData) {
  if (pKeyboardData == nullptr) {
    return KEYBOARD_RESULT_INVALID_ARGUMENT;
  }
  pKeyboardData->dev_handle = libusb_open_device_with_vid_pid(pKeyboardData->ctx, KEYBOARD_VID, KEYBOARD_PID);
  if (pKeyboardData->dev_handle == nullptr) {
    return KEYBOARD_RESULT_DEVICE_OPEN_FAILED;
  }
  if (libusb_set_auto_detach_kernel_driver(pKeyboardData->dev_handle, 1) < 0) {
    libusb_close(pKeyboardData->dev_handle);
    pKeyboardData->dev_handle = nullptr;
    return KEYBOARD_RESULT_AUTO_DETACH_IFACE3_FAILED;
  }
  if (libusb_claim_interface(pKeyboardData->dev_handle, KEYBOARD_IFACE3) < 0) {
    libusb_close(pKeyboardData->dev_handle);
    pKeyboardData->dev_handle = nullptr;
    return KEYBOARD_RESULT_CLAIM_IFACE3_FAILED;
  }
  return KEYBOARD_RESULT_OK;
}

[[nodiscard]] KeyboardResult initLibusb(KeyboardData *const pKeyboardData) {
  if (pKeyboardData == nullptr) {
    return KEYBOARD_RESULT_INVALID_ARGUMENT;
  }
  if (libusb_init(&pKeyboardData->ctx) < 0) {
    return KEYBOARD_RESULT_INIT_FAILED;
  }
  return KEYBOARD_RESULT_OK;
}
