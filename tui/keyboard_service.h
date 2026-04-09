#pragma once

#include "fusion_lib/api.h"

#include <string>

class KeyboardService {
public:
  const std::string &lastError() const;

  bool getCurrentMode(ModeData &outMode);
  bool setMode(const ModeData &modeData);

private:
  Api api_{};
  std::string lastError_;
};
