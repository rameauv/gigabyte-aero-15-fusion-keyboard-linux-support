#include "tui/mode_ui_mapping.h"

#include "tui/mode_constants.h"
#include "tui/my_app_component.h" // UiState

#include <algorithm>
#include <cstdint>

void uiToModeData(const UiState &state, ModeData &out) {
  out.mode       = static_cast<std::uint8_t>(MODE_ENTRIES.at(state.selectedMode).value);
  out.color      = static_cast<std::uint8_t>(MODE_COLOR_ENTRIES.at(state.selectedModeColor).value);
  out.brightness = static_cast<std::uint8_t>(std::clamp(state.selectedBrightness, 0, 50));
  out.speed      = static_cast<std::uint8_t>(std::clamp(10 - state.selectedSpeed, 0, 10));
}

namespace {
int findModeIndex(std::uint8_t modeValue) {
  for (std::size_t i = 0; i < MODE_ENTRIES.size(); ++i) {
    if (static_cast<std::uint8_t>(MODE_ENTRIES[i].value) == modeValue) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

int findModeColorIndex(std::uint8_t colorValue) {
  for (std::size_t i = 0; i < MODE_COLOR_ENTRIES.size(); ++i) {
    if (static_cast<std::uint8_t>(MODE_COLOR_ENTRIES[i].value) == colorValue) {
      return static_cast<int>(i);
    }
  }
  return -1;
}
} // namespace

void modeDataToUi(const ModeData &modeData, UiState &state) {
  const int modeIndex  = findModeIndex(modeData.mode);
  const int colorIndex = findModeColorIndex(modeData.color);

  bool hasUnknownValues = false;
  if (modeIndex >= 0) {
    state.selectedMode = modeIndex;
  } else {
    state.selectedMode = 0;
    hasUnknownValues   = true;
  }
  if (colorIndex >= 0) {
    state.selectedModeColor = colorIndex;
  } else {
    state.selectedModeColor = 0;
    hasUnknownValues        = true;
  }
  state.selectedBrightness = std::clamp(static_cast<int>(modeData.brightness), 0, 50);
  state.selectedSpeed      = std::clamp(static_cast<int>(10 - modeData.speed), 0, 10);

  if (hasUnknownValues) {
    state.status = "Device returned unknown mode/color; defaulted to first entries";
  } else {
    state.status = "Synced from device";
  }
}
