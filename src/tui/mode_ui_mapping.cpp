#include "src/tui/mode_ui_mapping.h"

#include "src/tui/mode_constants.h"
#include "src/tui/my_app_component.h"

#include <algorithm>
#include <cstdint>

namespace {
[[nodiscard]] auto findModeIndex(std::uint8_t modeValue) -> int;
[[nodiscard]] auto findModeColorIndex(std::uint8_t colorValue) -> int;
} // namespace

void uiToModeData(const UiState &state, ModeData &out) {
  if (kModeEntries.empty() || kModeColorEntries.empty()) {
    return;
  }
  const int max_mode_idx  = static_cast<int>(kModeEntries.size()) - 1;
  const int max_color_idx = static_cast<int>(kModeColorEntries.size()) - 1;
  const int mode_index    = std::clamp(state.selectedMode, 0, max_mode_idx);
  const int color_index   = std::clamp(state.selectedModeColor, 0, max_color_idx);
  const int brightness    = std::clamp(state.selectedBrightness, FUSION_BRIGHTNESS_MIN, FUSION_BRIGHTNESS_MAX);
  const int ui_speed      = std::clamp(state.selectedSpeed, FUSION_SPEED_MIN, FUSION_SPEED_MAX);
  const int mode_speed    = FUSION_SPEED_MAX - ui_speed;

  out.mode       = kModeEntries.at(static_cast<std::size_t>(mode_index)).value;
  out.color      = kModeColorEntries.at(static_cast<std::size_t>(color_index)).value;
  out.brightness = static_cast<std::uint8_t>(brightness);
  out.speed      = static_cast<std::uint8_t>(mode_speed);
}

void modeDataToUi(const ModeData &modeData, UiState &state) {
  const int mode_index  = findModeIndex(modeData.mode);
  const int color_index = findModeColorIndex(modeData.color);

  bool has_unknown_values = false;
  if (mode_index >= 0) {
    state.selectedMode = mode_index;
  } else {
    state.selectedMode = 0;
    has_unknown_values = true;
  }
  if (color_index >= 0) {
    state.selectedModeColor = color_index;
  } else {
    state.selectedModeColor = 0;
    has_unknown_values      = true;
  }
  state.selectedBrightness
      = std::clamp(static_cast<int>(modeData.brightness), FUSION_BRIGHTNESS_MIN, FUSION_BRIGHTNESS_MAX);
  state.selectedSpeed
      = std::clamp(static_cast<int>(FUSION_SPEED_MAX - modeData.speed), FUSION_SPEED_MIN, FUSION_SPEED_MAX);

  if (has_unknown_values) {
    state.status = "Device returned unknown mode/color; defaulted to first entries";
  } else {
    state.status = "Synced from device";
  }
}

namespace {
[[nodiscard]] auto findModeIndex(std::uint8_t modeValue) -> int {
  for (std::size_t i = 0; i < kModeEntries.size(); ++i) {
    if (kModeEntries.at(i).value == modeValue) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

[[nodiscard]] auto findModeColorIndex(std::uint8_t colorValue) -> int {
  for (std::size_t i = 0; i < kModeColorEntries.size(); ++i) {
    if (kModeColorEntries.at(i).value == colorValue) {
      return static_cast<int>(i);
    }
  }
  return -1;
}
} // namespace
