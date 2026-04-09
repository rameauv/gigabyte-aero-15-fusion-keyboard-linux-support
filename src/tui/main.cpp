#include "src/tui/mode_ui_mapping.h"
#include "src/tui/my_app_component.h"

#include <memory>

auto main() -> int {
  std::shared_ptr<KeyboardService> service = std::make_shared<KeyboardService>();
  UiState initial_state;
  ModeData current_mode_data{};
  if (!service->getCurrentMode(current_mode_data)) {
    return -1;
  }
  modeDataToUi(current_mode_data, initial_state);
  MyAppComponent my_app_component = MyAppComponent(initial_state, service);
  my_app_component.loop();

  return 0;
}
