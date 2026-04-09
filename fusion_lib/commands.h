#pragma once

#include <libusb.h>

#pragma pack(push, 1)
typedef struct ModeData {
  uint8_t mode;
  uint8_t speed;
  uint8_t brightness;
  uint8_t color;
} ModeData;
#pragma pack(pop)

typedef enum CmdSetModeResult {
  CMD_SET_MODE_RESULT_OK,
  CMD_SET_MODE_RESULT_CTRL_TRANSFER_FAIL_1,
  CMD_SET_MODE_RESULT_CTRL_TRANSFER_FAIL_2
} CmdSetModeResult;

typedef enum CmdGetCurrModeResult {
  CMD_GET_CURR_MODE_RESULT_OK,
  CMD_GET_CURR_MODE_RESULT_CTRL_TRANSFER_FAIL_1,
  CMD_GET_CURR_MODE_RESULT_CTRL_TRANSFER_FAIL_2
} CmdGetCurrModeResult;

typedef enum CmdSetCustomModeResult {
  CMD_SET_CUSTOM_MODE_RESULT_OK,
  CMD_SET_CUSTOM_MODE_RESULT_CTRL_TRANSFER_FAIL_1,
  CMD_SET_CUSTOM_MODE_RESULT_CTRL_TRANSFER_FAIL_2
} CmdSetCustomModeResult;

CmdGetCurrModeResult cmdGetCurrMode(libusb_device_handle *pHandle, ModeData *pOutModeData);

CmdSetModeResult cmdSetMode(libusb_device_handle *pHandle, uint8_t mode, uint8_t color, uint8_t brightness,
                            uint8_t speed);

CmdSetCustomModeResult cmdSetCustomMode(libusb_device_handle *pHandle, uint8_t *pData);
