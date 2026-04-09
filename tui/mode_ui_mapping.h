#pragma once

struct ModeData;
struct UiState;

void modeDataToUi(const ModeData& modeData, UiState& state);
void uiToModeData(const UiState& state, ModeData& out);

