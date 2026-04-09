#pragma once

#include "src/tui/keyboard_service.h"
#include "src/tui/mode_ui_mapping.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <memory>
#include <string>
#include <vector>

using UiState = struct UiState {
  int selectedMode;
  int selectedModeColor;
  int selectedBrightness;
  int selectedSpeed;
  std::string status;
};

class MyAppComponent {
public:
  MyAppComponent(UiState &state, std::shared_ptr<KeyboardService> service);
  void loop();

private:
  void apply();
  void refresh();

  UiState state_;
  std::shared_ptr<KeyboardService> service_;
  std::vector<std::string> modes_;
  std::vector<std::string> colors_;
  ftxui::ScreenInteractive screen_;
  ftxui::Component modeRadiobox_;
  ftxui::Component modeColorRadiobox_;
  ftxui::Component brightnessSlider_;
  ftxui::Component speedSlider_;
  ftxui::Component applyButton_;
  ftxui::Component refreshButton_;
  ftxui::Component quitButton_;
  ftxui::Component container_;
  ftxui::Component renderer_;
};
