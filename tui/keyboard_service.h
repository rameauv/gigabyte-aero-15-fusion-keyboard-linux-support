#pragma once

#include "fusion_lib/commands.h"

#include <string>

class KeyboardService {
public:
  const std::string &lastError() const;

  bool getCurrentMode(ModeData &outMode);
  bool setMode(const ModeData &modeData);

private:
  std::string lastError_;
};
