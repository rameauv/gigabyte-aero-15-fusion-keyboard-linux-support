#include "tui/my_app_component.h"

#include "tui/mode_constants.h"
#include "tui/mode_ui_mapping.h"

#include <ftxui/dom/elements.hpp>
#include <ranges>
#include <utility>

MyAppComponent::MyAppComponent(UiState &state, std::shared_ptr<KeyboardService> service)
    : service_(std::move(service)), screen_(ftxui::ScreenInteractive::FitComponent()) {
  using namespace ftxui;
  this->state_ = state;
  this->modes_ = MODE_ENTRIES
                 | std::views::transform([](const ModeEntry &n) { return n.label; })
                 | std::ranges::to<std::vector<std::string>>();

  this->colors_ = MODE_COLOR_ENTRIES
                  | std::views::transform([](const ModeColorEntry &n) { return n.label; })
                  | std::ranges::to<std::vector<std::string>>();

  this->modeRadiobox      = Radiobox(&this->modes_, &this->state_.selectedMode);
  this->modeColorRadiobox = Radiobox(&this->colors_, &this->state_.selectedModeColor);
  this->brightnessSlider  = Slider("Brightness:", &this->state_.selectedBrightness, 0, 50, 1);
  this->speedSlider       = Slider("Speed:     ", &this->state_.selectedSpeed, 0, 10, 1);

  auto apply = [this] {
    if (this->service_ == nullptr) {
      this->state_.status = "Service is not ready";
      return;
    }
    ModeData modeData{};
    uiToModeData(this->state_, modeData);
    if (!this->service_->setMode(modeData)) {
      this->state_.status = this->service_->lastError();
      return;
    }
    this->state_.status = "Applied successfully";
  };
  auto refresh = [this] {
    if (this->service_ == nullptr) {
      this->state_.status = "Service is not ready";
      return;
    }
    ModeData currentModeData{};
    if (!this->service_->getCurrentMode(currentModeData)) {
      this->state_.status = this->service_->lastError();
      return;
    }
    modeDataToUi(currentModeData, this->state_);
  };
  this->applyButton   = Button("Apply", apply, ButtonOption::Animated());
  this->refreshButton = Button("Refresh", refresh, ButtonOption::Animated());
  this->quitButton    = Button("Quit", this->screen_.ExitLoopClosure(), ButtonOption::Animated());

  this->container = Container::Vertical({
      Container::Horizontal({this->modeRadiobox, this->modeColorRadiobox}),
      this->brightnessSlider,
      this->speedSlider,
      this->applyButton,
      this->refreshButton,
      this->quitButton,
  });

  this->renderer = Renderer(container, [this] {
    return vbox({
        hbox({
            vbox({text("Mode"), this->modeRadiobox->Render()}) | borderDashed,
            vbox({text("Color"), this->modeColorRadiobox->Render()}) | borderDashed,
        }),
        this->brightnessSlider->Render(),
        this->speedSlider->Render(),
        this->applyButton->Render(),
        this->refreshButton->Render(),
        text(this->state_.status),
        this->quitButton->Render(),
    });
  });
}

void MyAppComponent::loop() { this->screen_.Loop(this->renderer); }
