#pragma once

#include "fusion_lib/api.h"

#include <string>

class KeyboardService {
public:
  KeyboardService();
  ~KeyboardService();

  bool isReady() const;
  const std::string &lastError() const;

  bool getCurrentMode(ModeData &outMode);
  bool setMode(const ModeData &modeData);

private:
  Api api_{};
  bool isReady_{false};
  std::string lastError_;
};
