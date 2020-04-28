#include "LedModes.h"

#include "LedModeStatus.h"
#include "Bars.h"
#include "MultiBars.h"
#include "Fireworks.h"
#include "LedModeSample.h"
#include "LedModeHsiboy.h"

LedMode *createModeStatus(LedMatrix& matrix) { return new LedModeStatus(matrix); }
LedMode *createModeBars(LedMatrix& matrix) { return new Bars(matrix); }
LedMode *createModeMultiBars(LedMatrix& matrix) { return new MultiBars(matrix); }
LedMode *createModeFireworks(LedMatrix& matrix) { return new Fireworks(matrix); }
LedMode *createModeSample(LedMatrix& matrix) { return new LedModeSample(matrix); }
LedMode *createModeHsiboy(LedMatrix& matrix) { return new LedModeHsiboy(matrix); }

LedModeDef_t modeStatusDef {"Status", &createModeStatus};
LedModeDef_t modeBarsDef {"Bars", &createModeBars};
LedModeDef_t modeMultiBarsDef {"MultiBars", &createModeMultiBars};
LedModeDef_t modeFireworksDef {"Fireworks", &createModeFireworks};
LedModeDef_t modeSampleDef {"Sample", &createModeSample};
LedModeDef_t modeHsiboyDef {"Hsiboy", &createModeHsiboy};

std::vector<LedModeDef_t> LedModes {
        modeStatusDef,
        modeBarsDef,
        modeMultiBarsDef,
        modeFireworksDef,
        modeSampleDef,
        modeHsiboyDef,
};
