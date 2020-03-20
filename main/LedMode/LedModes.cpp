#include "LedModes.h"

#include "LedModeStatus.h"
#include "Bars.h"
#include "MultiBars.h"
#include "Fireworks.h"
#include "LedModeSample.h"
#include "LedModeHsiboy.h"

LedMode *createModeStatus() { return new LedModeStatus(); }
LedMode *createModeBars() { return new Bars(); }
LedMode *createModeMultiBars() { return new MultiBars(); }
LedMode *createModeFireworks() { return new Fireworks(); }
LedMode *createModeSample() { return new LedModeSample(); }
LedMode *createModeHsiboy() { return new LedModeHsiboy(); }

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
