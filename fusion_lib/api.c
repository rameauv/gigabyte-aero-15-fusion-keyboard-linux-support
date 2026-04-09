#include "api.h"

#include "commands.h"
#include "enums.h"

#include <libusb.h>
#include <stdio.h>

static ApiBool isValidRgbKeyboard(libusb_device *pDevice);
static libusb_device_handle *getDeviceHandle(Api *pApi);
static int initDevice(Api *pApi);
static int initLibusb(Api *pApi);

#define KEYBOARD_VID 0x1044
#define KEYBOARD_PID 0x7a39

int apiInit(Api *pApi) {
  if (pApi == NULL) {
    return -1;
  }
  int r = 0;

  if (pApi->_init)
    return API_OK;
  pApi->_ctx       = NULL;
  pApi->dev_handle = NULL;
  r                = initLibusb(pApi);
  if (r != 0)
    return r;
  r = initDevice(pApi);
  if (r != 0)
    return r;
  printf("native: init done\n");
  pApi->_init = API_TRUE;
  return API_OK;
}

int apiUninit(Api *pApi) {
  if (pApi == NULL) {
    return -1;
  }
  libusb_release_interface(pApi->dev_handle, 0);
  libusb_release_interface(pApi->dev_handle, 3);
  libusb_close(pApi->dev_handle);
  libusb_exit(pApi->_ctx);
  pApi->_ctx       = NULL;
  pApi->dev_handle = NULL;
  printf("native: uninit done\n");
  return API_OK;
}

int apiGetCurrMode(Api *pApi, ModeData *pOutMode) {
  if (pOutMode == NULL || pApi == NULL) {
    return -1;
  }
  if (cmdGetCurrMode(pApi->dev_handle, pOutMode) != CMD_GET_CURR_MODE_RESULT_OK) {
    return -1;
  }
  return API_OK;
}

int apiSetMode(ModeData *pMode, Api *pApi) {
  if (pMode == NULL || pApi == NULL) {
    return -1;
  }
  if (cmdSetMode(pApi->dev_handle, pMode->mode, pMode->color, pMode->brightness, pMode->speed)
      != CMD_SET_MODE_RESULT_OK) {
    return -1;
  }
  return API_OK;
}

static int initDevice(Api *pApi) {
  if (pApi == NULL) {
    return -1;
  }
  int r            = 0;
  pApi->dev_handle = libusb_open_device_with_vid_pid(pApi->_ctx, KEYBOARD_VID, KEYBOARD_PID);
  if (pApi->dev_handle == NULL) {
    printf("Failed to open device!\n");
    libusb_exit(pApi->_ctx);
    return 1;
  }
  if (libusb_set_auto_detach_kernel_driver(pApi->dev_handle, 0) < 0) {
    printf("Kernel ctrl driver auto detach failed.\n");
    return -1;
  }
  if (libusb_set_auto_detach_kernel_driver(pApi->dev_handle, 3) < 0) {
    printf("Kernel driver auto detach failed.\n");
    return -1;
  }

  r = libusb_claim_interface(pApi->dev_handle, 0);
  if (r < 0) {
    printf("Failed to claim ctrl interface! %d\n", r);
    return 4;
  }
  r = libusb_claim_interface(pApi->dev_handle, 3);
  if (r < 0) {
    printf("Failed to claim interface! %d\n", r);
    return 2;
  }
  printf("init device done\n");
  return API_OK;
}

static int initLibusb(Api *pApi) {
  if (pApi == NULL) {
    return -1;
  }
  int r = libusb_init(&pApi->_ctx);
  // r = libusb_set_option(_ctx, LIBUSB_OPTION_USE_USBDK);
  if (r < 0) {
    printf("libusb_init error %d\n", r);
    return 1;
  }
  return API_OK;
}
