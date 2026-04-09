#include "commands.h"

#include "enums.h"

#include <stdio.h>

static void finHeader(HeaderT *pHdr);

CmdSetModeResult cmdSetMode(libusb_device_handle *pHandle, uint8_t mode,
                            uint8_t color, uint8_t brightness, uint8_t speed) {
  HeaderT header;
  header.kind        = KIND_PRESET;
  header.reserved    = RESERVED1;
  header.mode        = 0x33;
  header.speedLength = 0x05;
  header.brightness  = 0x32;
  header.color       = 0x02;
  header.reserved2   = 0x01;
  finHeader(&header);

  int r = libusb_control_transfer(pHandle, 0x21, 0x09, 0x0300, 0x0003,
                                  (uint8_t *)&header, 0x0008, 0);
  if (r < 0) {
    // printf("Control transfer 1 failed! %d\n", r);
    return CMD_SET_MODE_RESULT_CTRL_TRANSFER_FAIL_1;
  } else {
    // printf("Control transfer 1 OK!\n");
  }

  header.mode        = mode;
  header.speedLength = speed;
  header.brightness  = brightness;
  header.color       = color;
  header.reserved2   = 0;
  finHeader(&header);
  r = libusb_control_transfer(pHandle, 0x21, 0x09, 0x0300, 0x0003,
                              (uint8_t *)&header, 0x0008, 0);
  if (r < 0) {
    // printf("Control transfer 2 failed! %d\n", r);
    return CMD_SET_MODE_RESULT_CTRL_TRANSFER_FAIL_2;
  } else {
    // printf("Control transfer 2 OK!\n");
  }
  return CMD_SET_MODE_RESULT_OK;
}

CmdGetCurrModeResult cmdGetCurrMode(libusb_device_handle *pHandle,
                                    ModeData *pOutModeData) {
  HeaderT header;
  header.kind        = 0x88;
  header.reserved    = 0x00;
  header.mode        = 0x00;
  header.speedLength = 0x00;
  header.brightness  = 0x00;
  header.color       = 0x00;
  header.reserved2   = 0x00;
  finHeader(&header);
  printf("get current mode %p \n", pHandle);
  int r = libusb_control_transfer(pHandle, 0x21, 0x09, 0x0300, 0x0003,
                                  (uint8_t *)&header, 0x0008, 0);
  if (r < 0) {
    // printf("Control transfer 2 failed! %d\n", r);
    return CMD_GET_CURR_MODE_RESULT_CTRL_TRANSFER_FAIL_1;
  } else {
    // printf("Control transfer 2 OK!\n");
  }

  header.kind        = 0x00;
  header.reserved    = 0x00;
  header.mode        = 0x00;
  header.speedLength = 0x00;
  header.brightness  = 0x00;
  header.color       = 0x00;
  header.reserved2   = 0x00;
  header.checksum    = 0x00;

  r = libusb_control_transfer(pHandle, 0xa1, 0x01, 0x0300, 0x0003,
                              (uint8_t *)&header, 0x0008, 0);
  if (r < 0) {
    // printf("Control transfer 1 failed! %d\n", r);
    return CMD_GET_CURR_MODE_RESULT_CTRL_TRANSFER_FAIL_2;
  } else {
    // printf("Control transfer 1 OK!\n");
  }
  pOutModeData->mode       = header.mode;
  pOutModeData->speed      = header.speedLength;
  pOutModeData->brightness = header.brightness;
  pOutModeData->color      = header.color;
  return CMD_GET_CURR_MODE_RESULT_OK;
}

CmdSetCustomModeResult cmdSetCustomMode(libusb_device_handle *pHandle,
                                        uint8_t *pData) {
  HeaderT header;
  header.kind        = KIND_CUSTOM_CONFIG;
  header.reserved    = RESERVED1;
  header.mode        = 1;
  header.speedLength = 8;
  header.brightness  = 0;
  header.color       = 0;
  header.reserved2   = RESERVED1;
  finHeader(&header);

  int r = libusb_control_transfer(pHandle, 0x21, 0x09, 0x0300, 0x0003,
                                  (uint8_t *)&header, 0x0008, 0);
  if (r < 0) {
    // printf("Control transfer 1 failed! %d\n", r);
    return CMD_SET_CUSTOM_MODE_RESULT_CTRL_TRANSFER_FAIL_1;
  } else {
    // printf("Control transfer 1 OK!\n");
  }

  for (uint8_t i = 0; i < 8; i++) {
    int transferred = 0;
    r = libusb_interrupt_transfer(pHandle, (6 | LIBUSB_ENDPOINT_OUT),
                                  pData + (i * 64), 64, &transferred, 0);
    if (r < 0 || transferred != 64) {
      printf("Interrupt transfer %d failed", i);
    }
  }
  // printf("Interrupt transfers OK!\n");

  header.kind        = KIND_PRESET;
  header.reserved    = RESERVED1;
  header.mode        = 0x34;
  header.speedLength = 0x05;
  header.brightness  = 0x32;
  header.color       = 0x02;
  header.reserved2   = 0x01;
  finHeader(&header);

  r = libusb_control_transfer(pHandle, 0x21, 0x09, 0x0300, 0x0003,
                              (uint8_t *)&header, 0x0008, 0);
  if (r < 0) {
    // printf("Control transfer 2 failed! %d\n", r);
    return CMD_SET_CUSTOM_MODE_RESULT_CTRL_TRANSFER_FAIL_2;
  } else {
    // printf("Control transfer 2 OK!\n");
  }
  return 0;
}

void finHeader(HeaderT *hdr) {
  hdr->reserved      = 0;
  uint16_t chksumtmp = 0;
  uint8_t *data      = (uint8_t *)hdr;
  for (uint8_t i = 0; i < 7; i++) {
    chksumtmp += data[i];
  }
  hdr->checksum = (uint8_t)(0xff - (chksumtmp & 0xff));
}
