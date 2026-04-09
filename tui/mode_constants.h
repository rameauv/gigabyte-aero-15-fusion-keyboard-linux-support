#pragma once

#include "fusion_lib/enums.h"

#include <array>
#include <string>

struct ModeEntry {
  Mode value;
  std::string_view label;
};

struct ModeColorEntry {
  ModeColor value;
  std::string_view label;
};

inline constexpr std::array MODE_ENTRIES = {
    ModeEntry{.value = MODE_STATIC, .label = "STATIC"},
    ModeEntry{.value = MODE_BREATHING, .label = "BREATHING"},
    ModeEntry{.value = MODE_WAVE, .label = "WAVE"},
    ModeEntry{.value = MODE_FADE_ON_KEYPRESS, .label = "FADE_ON_KEYPRESS"},
    ModeEntry{.value = MODE_MARQUEE, .label = "MARQUEE"},
    ModeEntry{.value = MODE_RIPPLE, .label = "RIPPLE"},
    ModeEntry{.value = MODE_FLASH_ON_KEYPRESS, .label = "FLASH_ON_KEYPRESS"},
    ModeEntry{.value = MODE_NEON, .label = "NEON"},
    ModeEntry{.value = MODE_RAINBOW_MARQUEE, .label = "RAINBOW_MARQUEE"},
    ModeEntry{.value = MODE_RAINDROP, .label = "RAINDROP"},
    ModeEntry{.value = MODE_CIRCLE_MARQUEE, .label = "CIRCLE_MARQUEE"},
    ModeEntry{.value = MODE_HEDGE, .label = "HEDGE"},
    ModeEntry{.value = MODE_ROTATE, .label = "ROTATE"},
};

inline constexpr std::array MODE_COLOR_ENTRIES = {
    ModeColorEntry{.value = MODE_COLOR_RED, .label = "RED"},
    ModeColorEntry{.value = MODE_COLOR_GREEN, .label = "GREEN"},
    ModeColorEntry{.value = MODE_COLOR_YELLOW, .label = "YELLOW"},
    ModeColorEntry{.value = MODE_COLOR_BLUE, .label = "BLUE "},
    ModeColorEntry{.value = MODE_COLOR_PURPLE, .label = "PURPLE"},
    ModeColorEntry{.value = MODE_COLOR_AQUA, .label = "AQUA "},
    ModeColorEntry{.value = MODE_COLOR_WHITE, .label = "WHITE"},
};
