#pragma once

#include "src/fusion_lib/api.h"

#include <string>

class KeyboardService {
public:
  [[nodiscard]] auto lastError() const -> const std::string &;

  [[nodiscard]] auto getCurrentMode(ModeData &outMode) -> bool;
  [[nodiscard]] auto setMode(const ModeData &modeData) -> bool;

private:
  std::string lastError_;
};
