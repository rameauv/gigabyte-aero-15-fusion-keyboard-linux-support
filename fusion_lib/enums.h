#pragma once

#include <stdint.h>

#define RESERVED1 0x00

typedef enum Kind : uint8_t {
  KIND_PRESET        = 0x08,
  KIND_READ_CONFIG   = 0x92,
  KIND_CUSTOM_CONFIG = 0x12,
} Kind;

typedef enum ModeColor : uint8_t {
  MODE_COLOR_RED    = 0x01,
  MODE_COLOR_GREEN  = 0x02,
  MODE_COLOR_YELLOW = 0x03,
  MODE_COLOR_BLUE   = 0x04,
  MODE_COLOR_PURPLE = 0x05,
  MODE_COLOR_AQUA   = 0x06,
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

#define API_OK (0)

typedef int ApiBool;
enum { API_FALSE, API_TRUE };
