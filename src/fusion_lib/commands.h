#pragma once

#include "api.h"

#include <libusb.h>

#pragma pack(push, 1)
typedef struct {
  // Kind of the control transfer
  uint8_t kind;
  // ?? always zeroed
  uint8_t reserved;
  // mode or config slot
  uint8_t mode;
  // Speed or length of usb packets to follow
  uint8_t speedLength;
  // Brightness. 0 to 50
  uint8_t brightness;
  // Predefined color
  uint8_t color;
  uint8_t reserved2;
  uint8_t checksum;
} HeaderT;
#pragma pack(pop)

typedef enum CmdSetModeResult {
  CMD_SET_MODE_RESULT_OK = 0,
  CMD_SET_MODE_RESULT_CTRL_TRANSFER_FAIL_1,
  CMD_SET_MODE_RESULT_CTRL_TRANSFER_FAIL_2,
  CMD_SET_MODE_RESULT_FAIL
} CmdSetModeResult;

typedef enum CmdGetCurrModeResult {
  CMD_GET_CURR_MODE_RESULT_OK = 0,
  CMD_GET_CURR_MODE_RESULT_CTRL_TRANSFER_FAIL_1,
  CMD_GET_CURR_MODE_RESULT_CTRL_TRANSFER_FAIL_2,
  CMD_GET_CURR_MODE_RESULT_FAIL
} CmdGetCurrModeResult;

typedef enum CmdSetCustomModeResult {
  CMD_SET_CUSTOM_MODE_RESULT_OK = 0,
  CMD_SET_CUSTOM_MODE_RESULT_CTRL_TRANSFER_FAIL_1,
  CMD_SET_CUSTOM_MODE_RESULT_INTERRUPT_TRANSFER_FAIL,
  CMD_SET_CUSTOM_MODE_RESULT_CTRL_TRANSFER_FAIL_2,
  CMD_SET_CUSTOM_MODE_RESULT_FAIL
} CmdSetCustomModeResult;

[[nodiscard]] CmdGetCurrModeResult cmdGetCurrMode(libusb_device_handle *pHandle, ModeData *pOutModeData);

[[nodiscard]] CmdSetModeResult cmdSetMode(libusb_device_handle *pHandle, const ModeData *pModeData);

[[nodiscard]] CmdSetCustomModeResult cmdSetCustomMode(libusb_device_handle *pHandle, uint8_t *pData);
