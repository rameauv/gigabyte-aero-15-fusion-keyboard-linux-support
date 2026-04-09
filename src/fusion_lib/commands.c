#include "commands.h"

#include <stdint.h>
#include <threads.h>
#include <time.h>

#define HEADER_RESERVED1 0x00
#define HEADER_RESERVED2 0x00

typedef enum HeaderKind : uint8_t {
  HEADER_KIND_PRESET        = 0x08,
  HEADER_KIND_GET_CURR_MODE = 0x88,
} HeaderKind;

typedef enum HidRequestType {
  HID_GET_REPORT = 0x01,
  HID_SET_REPORT = 0x09,
} HidRequestType;

typedef enum HidReportType {
  HID_REPORT_TYPE_FEATURE = 0x03,
} HidReportType;

#define REPORT_ID 0x00
#define KEYBOARD_WVALUE ((HID_REPORT_TYPE_FEATURE << 8) | REPORT_ID)
#define HEADER_SIZE 8
#define LIBUSB_TIMEOUT_IN_MS 5000

[[nodiscard]] static int finHeader(HeaderT *pHdr);

[[nodiscard]] CmdSetModeResult cmdSetMode(libusb_device_handle *const pHandle, const ModeData *const pModeData) {
  if (pHandle == nullptr || pModeData == nullptr) {
    return CMD_SET_MODE_RESULT_FAIL;
  }
  HeaderT header;
  header.kind        = HEADER_KIND_PRESET;
  header.reserved    = HEADER_RESERVED1;
  header.mode        = pModeData->mode;
  header.speedLength = pModeData->speed;
  header.brightness  = pModeData->brightness;
  header.color       = pModeData->color;
  header.reserved2   = HEADER_RESERVED2;
  if (finHeader(&header) != 0) {
    return CMD_SET_MODE_RESULT_FAIL;
  }
  if (libusb_control_transfer(
          pHandle,
          (uint8_t)LIBUSB_REQUEST_TYPE_CLASS | (uint8_t)LIBUSB_RECIPIENT_INTERFACE | (uint8_t)LIBUSB_ENDPOINT_OUT,
          HID_SET_REPORT, KEYBOARD_WVALUE, KEYBOARD_IFACE3, (uint8_t *)&header, HEADER_SIZE, LIBUSB_TIMEOUT_IN_MS)
      != HEADER_SIZE) {
    thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL);
    return CMD_SET_MODE_RESULT_CTRL_TRANSFER_FAIL_2;
  }
  thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL);
  return CMD_SET_MODE_RESULT_OK;
}

[[nodiscard]] CmdGetCurrModeResult cmdGetCurrMode(libusb_device_handle *const pHandle, ModeData *const pOutModeData) {
  if (pHandle == nullptr || pOutModeData == nullptr) {
    return CMD_GET_CURR_MODE_RESULT_FAIL;
  }
  HeaderT header;
  header.kind        = HEADER_KIND_GET_CURR_MODE;
  header.reserved    = 0x00;
  header.mode        = 0x00;
  header.speedLength = 0x00;
  header.brightness  = 0x00;
  header.color       = 0x00;
  header.reserved2   = HEADER_RESERVED2;
  if (finHeader(&header) != 0) {
    return CMD_GET_CURR_MODE_RESULT_FAIL;
  }
  if (libusb_control_transfer(
          pHandle,
          (uint8_t)LIBUSB_REQUEST_TYPE_CLASS | (uint8_t)LIBUSB_RECIPIENT_INTERFACE | (uint8_t)LIBUSB_ENDPOINT_OUT,
          HID_SET_REPORT, KEYBOARD_WVALUE, KEYBOARD_IFACE3, (uint8_t *)&header, HEADER_SIZE, LIBUSB_TIMEOUT_IN_MS)
      != HEADER_SIZE) {
    return CMD_GET_CURR_MODE_RESULT_CTRL_TRANSFER_FAIL_1;
  }

  header.kind        = 0x00;
  header.reserved    = 0x00;
  header.mode        = 0x00;
  header.speedLength = 0x00;
  header.brightness  = 0x00;
  header.color       = 0x00;
  header.reserved2   = 0x00;
  header.checksum    = 0x00;

  if (libusb_control_transfer(
          pHandle,
          (uint8_t)LIBUSB_REQUEST_TYPE_CLASS | (uint8_t)LIBUSB_RECIPIENT_INTERFACE | (uint8_t)LIBUSB_ENDPOINT_IN,
          HID_GET_REPORT, KEYBOARD_WVALUE, KEYBOARD_IFACE3, (uint8_t *)&header, HEADER_SIZE, LIBUSB_TIMEOUT_IN_MS)
      != HEADER_SIZE) {
    return CMD_GET_CURR_MODE_RESULT_CTRL_TRANSFER_FAIL_2;
  }
  pOutModeData->mode       = header.mode;
  pOutModeData->speed      = header.speedLength;
  pOutModeData->brightness = header.brightness;
  pOutModeData->color      = header.color;
  return CMD_GET_CURR_MODE_RESULT_OK;
}

[[nodiscard]] int finHeader(HeaderT *const pHdr) {
  if (pHdr == nullptr) {
    return -1;
  }
  pHdr->reserved     = 0;
  uint16_t chksumtmp = 0;
  uint8_t *data      = (uint8_t *)pHdr;
  for (uint8_t i = 0; i < HEADER_SIZE - 1; i++) {
    chksumtmp += data[i];
  }
  pHdr->checksum = (uint8_t)(0xff - (chksumtmp & 0xff));
  return 0;
}
