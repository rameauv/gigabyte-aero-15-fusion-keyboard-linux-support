#include "tui/my_app_component.h"
#include "tui/mode_ui_mapping.h"

#include <memory>

int main() {
  std::shared_ptr<KeyboardService> service = std::make_shared<KeyboardService>();
  UiState initialState;
  ModeData currentModeData;
  if (!service->getCurrentMode(currentModeData)) {
    return -1;
  }
  modeDataToUi(currentModeData, initialState);
  MyAppComponent myAppComponent = MyAppComponent(initialState, service);

  // myAppComponent.loop();

  return 0;
}
