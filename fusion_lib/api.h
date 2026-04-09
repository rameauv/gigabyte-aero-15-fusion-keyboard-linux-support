#pragma once

#include "commands.h"
#include "enums.h"

#include <libusb.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Api {
  libusb_context *_ctx;             // NULL
  libusb_device_handle *dev_handle; // NULL
  ApiBool _init;                    // false
} Api;

/**
 * @param pApi Api struct to initialize
 * @return API_OK on success
 */
int apiInit(Api *pApi);
/**
 * @param pApi Api struct to uninitialize
 * @return API_OK on success
 */
int apiUninit(Api *pApi);
/**
 * @param pMode the output mode
 * @param pApi
 * @return API_OK on success
 */
int apiGetCurrMode(Api *pApi, ModeData *pOutMode);
/**
 * @param pMode the mode to set
 * @param pApi
 * @return API_OK on success
 */
int apiSetMode(ModeData *pMode, Api *pApi);

#ifdef __cplusplus
}
#endif
