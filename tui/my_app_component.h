#pragma once

#include "tui/keyboard_service.h"
#include "tui/mode_ui_mapping.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <memory>
#include <string>
#include <vector>

typedef struct UiState {
  int selectedMode;
  int selectedModeColor;
  int selectedBrightness;
  int selectedSpeed;
  std::string status;
} UiState;

class MyAppComponent {
public:
  MyAppComponent(UiState &state, std::shared_ptr<KeyboardService> service);
  void loop();

private:
  UiState state_;
  std::shared_ptr<KeyboardService> service_;
  std::vector<std::string> modes_;
  std::vector<std::string> colors_;
  ftxui::ScreenInteractive screen_;

  ftxui::Component modeRadiobox;
  ftxui::Component modeColorRadiobox;
  ftxui::Component brightnessSlider;
  ftxui::Component speedSlider;
  ftxui::Component applyButton;
  ftxui::Component refreshButton;
  ftxui::Component quitButton;
  ftxui::Component container;
  ftxui::Component renderer;
};
