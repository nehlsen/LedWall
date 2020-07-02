#include "LedModes.h"
#include "LedModeStatus.h"
#include "Bars.h"
#include "MultiBars.h"
#include "Fireworks.h"
#include "LedModeSample.h"
#include "LedModeHsiboy.h"
#include "Fire.h"
#include "Network.h"
#include "ModeText.h"
#include "MatesDemo.h"
#include "Wave.h"

#ifdef CONFIG_ENABLE_CAM
#include "Camera.h"
#endif

namespace LedWall {
namespace Mode {

LedMode *createModeStatus(LedMatrix& matrix) { return new LedModeStatus(matrix); }
LedMode *createModeBars(LedMatrix& matrix) { return new Bars(matrix); }
LedMode *createModeMultiBars(LedMatrix& matrix) { return new MultiBars(matrix); }
LedMode *createModeFireworks(LedMatrix& matrix) { return new Fireworks(matrix); }
LedMode *createModeSample(LedMatrix& matrix) { return new LedModeSample(matrix); }
LedMode *createModeHsiboy(LedMatrix& matrix) { return new LedModeHsiboy(matrix); }
LedMode *createModeFire(LedMatrix& matrix) { return new Fire(matrix); }
LedMode *createModeNetwork(LedMatrix& matrix) { return new Network(matrix); }
LedMode *createModeText(LedMatrix& matrix) { return new ModeText(matrix); }
LedMode *createModeMatesDemo(LedMatrix& matrix) { return new MatesDemo(matrix); }
LedMode *createModeWave(LedMatrix& matrix) { return new Wave(matrix); }

LedModeDef_t modeStatusDef {"Status", &createModeStatus};
LedModeDef_t modeBarsDef {"Bars", &createModeBars};
LedModeDef_t modeMultiBarsDef {"MultiBars", &createModeMultiBars};
LedModeDef_t modeFireworksDef {"Fireworks", &createModeFireworks};
LedModeDef_t modeSampleDef {"Sample", &createModeSample};
LedModeDef_t modeHsiboyDef {"Hsiboy", &createModeHsiboy};
LedModeDef_t modeFireDef {"Fire", &createModeFire};
LedModeDef_t modeNetworkDef {"Network", &createModeNetwork};
LedModeDef_t modeTextDef {"Text", &createModeText};
LedModeDef_t modeMatesDemoDef {"MatesDemo", &createModeMatesDemo};
LedModeDef_t modeWaveDef {"Wave", &createModeWave};

#ifdef CONFIG_ENABLE_CAM
LedMode *createModeCamera(LedMatrix& matrix) { return new Camera(matrix); }
LedModeDef_t modeCameraDef {"Camera", &createModeCamera};
#endif

std::vector<LedModeDef_t> LedModes {
        modeStatusDef,
        modeBarsDef,
        modeMultiBarsDef,
        modeFireworksDef,
        modeSampleDef,
        modeHsiboyDef,
        modeFireDef,
        modeNetworkDef,
        modeTextDef,
        modeMatesDemoDef,
        modeWaveDef,
#ifdef CONFIG_ENABLE_CAM
        modeCameraDef,
#endif
};

} // namespace Mode
} // namespace LedWall
