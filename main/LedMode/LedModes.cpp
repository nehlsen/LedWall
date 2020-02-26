#include "LedModes.h"

#include "LedModeStatus.h"
#include "Bars.h"
#include "Fireworks.h"
#include "LedModeSample.h"
#include "LedModeHsiboy.h"

LedMode *createModeStatus(CRGB *leds, int ledCount) { return new LedModeStatus(leds, ledCount); }
LedMode *createModeBars(CRGB *leds, int ledCount) { return new Bars(leds, ledCount); }
LedMode *createModeFireworks(CRGB *leds, int ledCount) { return new Fireworks(leds, ledCount); }
LedMode *createModeSample(CRGB *leds, int ledCount) { return new LedModeSample(leds, ledCount); }
LedMode *createModeHsiboy(CRGB *leds, int ledCount) { return new LedModeHsiboy(leds, ledCount); }

LedModeDef_t modeStatusDef {"Status", &createModeStatus};
LedModeDef_t modeBarsDef {"Bars", &createModeBars};
LedModeDef_t modeFireworksDef {"Fireworks", &createModeFireworks};
LedModeDef_t modeSampleDef {"Sample", &createModeSample};
LedModeDef_t modeHsiboyDef {"Hsiboy", &createModeHsiboy};

std::vector<LedModeDef_t> LedModes {
        modeStatusDef,
        modeBarsDef,
        modeFireworksDef,
        modeSampleDef,
        modeHsiboyDef,
};
