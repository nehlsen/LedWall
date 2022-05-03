#include "LedModes.h"
#include "LedModeStatus.h"
#include "Bars.h"
#include "MultiBars.h"
#include "Fireworks.h"
#include "LedModeSample.h"
#include "LedModeHsiboy.h"
#include "Fire.h"
#include "ModeText.h"
#include "MatesDemo.h"
#include "Wave.h"
#include "FancyDemo.h"
#include "GameOfLife.h"
#include "ModeTime.h"

#ifdef CONFIG_ENABLE_CAM
#include "Camera.h"
#endif

// not configurable, just disabled
#ifdef CONFIG_MODE_NETWORK_ENABLED
#include "Network.h"
#endif

namespace LedWall::Mode {

LedMode *createModeStatus(LedMatrix& matrix) { return new LedModeStatus(matrix); }
LedMode *createModeBars(LedMatrix& matrix) { return new Bars(matrix); }
LedMode *createModeMultiBars(LedMatrix& matrix) { return new MultiBars(matrix); }
LedMode *createModeFireworks(LedMatrix& matrix) { return new Fireworks(matrix); }
LedMode *createModeSample(LedMatrix& matrix) { return new LedModeSample(matrix); }
LedMode *createModeHsiboy(LedMatrix& matrix) { return new LedModeHsiboy(matrix); }
LedMode *createModeFire(LedMatrix& matrix) { return new Fire(matrix); }
LedMode *createModeText(LedMatrix& matrix) { return new ModeText(matrix); }
LedMode *createModeMatesDemo(LedMatrix& matrix) { return new MatesDemo(matrix); }
LedMode *createModeWave(LedMatrix& matrix) { return new Wave(matrix); }
LedMode *createModeFancyDemo(LedMatrix& matrix) { return new FancyDemo(matrix); }
LedMode *createModeGameOfLife(LedMatrix& matrix) { return new GameOfLife(matrix); }
LedMode *createModeTime(LedMatrix& matrix) { return new ModeTime(matrix); }

LedModeDef_t modeStatusDef {"Status", &createModeStatus};
LedModeDef_t modeBarsDef {"Bars", &createModeBars};
LedModeDef_t modeMultiBarsDef {"MultiBars", &createModeMultiBars};
LedModeDef_t modeFireworksDef {"Fireworks", &createModeFireworks};
LedModeDef_t modeSampleDef {"Sample", &createModeSample};
LedModeDef_t modeHsiboyDef {"Hsiboy", &createModeHsiboy};
LedModeDef_t modeFireDef {"Fire", &createModeFire};
LedModeDef_t modeTextDef {"Text", &createModeText};
LedModeDef_t modeMatesDemoDef {"MatesDemo", &createModeMatesDemo};
LedModeDef_t modeWaveDef {"Wave", &createModeWave};
LedModeDef_t modeFancyDemoDef {"FancyDemo", &createModeFancyDemo};
LedModeDef_t modeGameOfLifeDef {"GameOfLife", &createModeGameOfLife};
LedModeDef_t modeTimeDef {"Time", &createModeTime};

#ifdef CONFIG_ENABLE_CAM
LedMode *createModeCamera(LedMatrix& matrix) { return new Camera(matrix); }
LedModeDef_t modeCameraDef {"Camera", &createModeCamera};
#endif

#ifdef CONFIG_MODE_NETWORK_ENABLED
LedMode *createModeNetwork(LedMatrix& matrix) { return new Network(matrix); }
LedModeDef_t modeNetworkDef {"Network", &createModeNetwork};
#endif

std::vector<LedModeDef_t> LedModes {
        modeStatusDef,
        modeBarsDef,
        modeMultiBarsDef,
        modeFireworksDef,
        modeSampleDef,
        modeHsiboyDef,
        modeFireDef,
        modeTextDef,
        modeMatesDemoDef,
        modeWaveDef,
        modeFancyDemoDef,
        modeGameOfLifeDef,
        modeTimeDef,
#ifdef CONFIG_ENABLE_CAM
        modeCameraDef,
#endif
#ifdef CONFIG_MODE_NETWORK_ENABLED
        modeNetworkDef,
#endif
};

} // namespace LedWall
