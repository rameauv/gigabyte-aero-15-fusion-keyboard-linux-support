#include "api.h"

#include "commands.h"
#include "enums.h"

#include <libusb.h>
#include <stdio.h>

static ApiBool isValidRgbKeyboard(libusb_device *pDevice);
static libusb_device_handle *getDeviceHandle(Api *pApi);
static int initDevice(Api *pApi);
static int initLibusb(Api *pApi);

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

static ApiBool isValidRgbKeyboard(libusb_device *pDevice) {
  int ret;
  struct libusb_device_descriptor desc;

  ret = libusb_get_device_descriptor(pDevice, &desc);
  if (ret < 0) {
    fprintf(stderr, "failed to get device descriptor");
    return (0);
  }
  printf("vid:%x, pid:%x\n", desc.idVendor, desc.idProduct);
  if (desc.idVendor == 0x1044 && desc.idProduct == 0x7A39)
    return 1;
  return 0;
}

static libusb_device_handle *getDeviceHandle(Api *pApi) {
  if (pApi == NULL) {
    return NULL;
  }
  libusb_device **devs;
  libusb_device *device;
  ssize_t cnt;
  int i;
  libusb_device_handle *devHandle = NULL;
  int ret                         = -15;

  cnt = libusb_get_device_list(NULL, &devs);
  if (cnt < 0) {
    return NULL;
  }

  for (i = 0; devs[i]; ++i) {
    if (isValidRgbKeyboard(devs[i])) {
      device = devs[i];
      for (int i = 0; i < 30 && ret != 0; i++) {
        printf("try to open device:%d\n", i);
        ret = libusb_open(device, &devHandle);
      }
      printf("ret_handle:%d\n", ret);
      if (LIBUSB_SUCCESS == ret) {
        return devHandle;
      }
      return NULL;
    }
  }

  libusb_free_device_list(devs, 1);
  return NULL;
}

static int initDevice(Api *pApi) {
  if (pApi == NULL) {
    return -1;
  }
  int r = 0;
  pApi->dev_handle = getDeviceHandle(pApi);
  // pApi->dev_handle = libusb_open_device_with_vid_pid(pApi->_ctx, 0x1044, 0x7a39);
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
