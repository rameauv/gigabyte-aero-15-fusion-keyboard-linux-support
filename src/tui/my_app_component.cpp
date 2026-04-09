#include "src/tui/my_app_component.h"

#include "src/tui/mode_constants.h"
#include "src/tui/mode_ui_mapping.h"

#include <ftxui/dom/elements.hpp>
#include <ranges>
#include <utility>

MyAppComponent::MyAppComponent(UiState &state, std::shared_ptr<KeyboardService> service)
    : state_(state), service_(std::move(service)), screen_(ftxui::ScreenInteractive::FitComponent()) {
  this->modes_ = kModeEntries
                 | std::views::transform([](const ModeEntry &entry) -> std::string_view { return entry.label; })
                 | std::ranges::to<std::vector<std::string>>();

  this->colors_ = kModeColorEntries
                  | std::views::transform([](const ModeColorEntry &entry) -> std::string_view { return entry.label; })
                  | std::ranges::to<std::vector<std::string>>();

  this->modeRadiobox_      = ftxui::Radiobox(&this->modes_, &this->state_.selectedMode);
  this->modeColorRadiobox_ = ftxui::Radiobox(&this->colors_, &this->state_.selectedModeColor);
  this->brightnessSlider_
      = ftxui::Slider("Brightness:", &this->state_.selectedBrightness, FUSION_BRIGHTNESS_MIN, FUSION_BRIGHTNESS_MAX, 1);
  this->speedSlider_ = ftxui::Slider("Speed:     ", &this->state_.selectedSpeed, FUSION_SPEED_MIN, FUSION_SPEED_MAX, 1);
  this->applyButton_ = Button("Apply", [this] -> void { this->apply(); }, ftxui::ButtonOption::Animated());
  this->refreshButton_ = Button("Refresh", [this] -> void { this->refresh(); }, ftxui::ButtonOption::Animated());
  this->quitButton_    = Button("Quit", this->screen_.ExitLoopClosure(), ftxui::ButtonOption::Animated());
  this->container_     = ftxui::Container::Vertical({
      ftxui::Container::Horizontal({this->modeRadiobox_, this->modeColorRadiobox_}),
      this->brightnessSlider_,
      this->speedSlider_,
      this->applyButton_,
      this->refreshButton_,
      this->quitButton_,
  });
  this->renderer_      = Renderer(container_, [this] -> ftxui::Element {
    return ftxui::vbox({
        ftxui::hbox({
            ftxui::vbox({ftxui::text("Mode"), this->modeRadiobox_->Render()}) | ftxui::borderDashed,
            ftxui::vbox({ftxui::text("Color"), this->modeColorRadiobox_->Render()}) | ftxui::borderDashed,
        }),
        this->brightnessSlider_->Render(),
        this->speedSlider_->Render(),
        this->applyButton_->Render(),
        this->refreshButton_->Render(),
        ftxui::text(this->state_.status),
        this->quitButton_->Render(),
    });
  });
}

void MyAppComponent::loop() { this->screen_.Loop(this->renderer_); }

void MyAppComponent::apply() {
  if (this->service_ == nullptr) {
    this->state_.status = "Service is not ready";
    return;
  }
  ModeData mode_data{};
  uiToModeData(this->state_, mode_data);
  if (!this->service_->setMode(mode_data)) {
    this->state_.status = this->service_->lastError();
    return;
  }
  this->state_.status = "Applied successfully";
}

void MyAppComponent::refresh() {
  if (this->service_ == nullptr) {
    this->state_.status = "Service is not ready";
    return;
  }
  ModeData current_mode_data{};
  if (!this->service_->getCurrentMode(current_mode_data)) {
    this->state_.status = this->service_->lastError();
    return;
  }
  modeDataToUi(current_mode_data, this->state_);
}
