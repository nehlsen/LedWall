#include "LedModes.h"

#include "LedModeStatus.h"
#include "Bars.h"
#include "MultiBars.h"
#include "Fireworks.h"
#include "LedModeSample.h"
#include "LedModeHsiboy.h"
#include "Fire.h"
#include "Camera.h"
#include "Network.h"
#include "Breathe.h"
#include "Text.h"

namespace LedWall {
namespace Mode {

LedMode *createModeStatus(LedMatrix& matrix) { return new LedModeStatus(matrix); }
LedMode *createModeBars(LedMatrix& matrix) { return new Bars(matrix); }
LedMode *createModeMultiBars(LedMatrix& matrix) { return new MultiBars(matrix); }
LedMode *createModeFireworks(LedMatrix& matrix) { return new Fireworks(matrix); }
LedMode *createModeSample(LedMatrix& matrix) { return new LedModeSample(matrix); }
LedMode *createModeHsiboy(LedMatrix& matrix) { return new LedModeHsiboy(matrix); }
LedMode *createModeFire(LedMatrix& matrix) { return new Fire(matrix); }
LedMode *createModeCamera(LedMatrix& matrix) { return new Camera(matrix); }
LedMode *createModeNetwork(LedMatrix& matrix) { return new Network(matrix); }
LedMode *createModeBreathe(LedMatrix& matrix) { return new Breathe(matrix); }
LedMode *createModeText(LedMatrix& matrix) { return new Text(matrix); }

LedModeDef_t modeStatusDef {"Status", &createModeStatus};
LedModeDef_t modeBarsDef {"Bars", &createModeBars};
LedModeDef_t modeMultiBarsDef {"MultiBars", &createModeMultiBars};
LedModeDef_t modeFireworksDef {"Fireworks", &createModeFireworks};
LedModeDef_t modeSampleDef {"Sample", &createModeSample};
LedModeDef_t modeHsiboyDef {"Hsiboy", &createModeHsiboy};
LedModeDef_t modeFireDef {"Fire", &createModeFire};
LedModeDef_t modeCameraDef {"Camera", &createModeCamera};
LedModeDef_t modeNetworkDef {"Network", &createModeNetwork};
LedModeDef_t modeBreatheDef {"Breathe", &createModeBreathe};
LedModeDef_t modeTextDef {"Text", &createModeText};

std::vector<LedModeDef_t> LedModes {
        modeStatusDef,
        modeBarsDef,
        modeMultiBarsDef,
        modeFireworksDef,
        modeSampleDef,
        modeHsiboyDef,
        modeFireDef,
        modeCameraDef,
        modeNetworkDef,
        modeBreatheDef,
        modeTextDef,
};

} // namespace Mode
} // namespace LedWall
