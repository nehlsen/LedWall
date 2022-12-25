#include "FancyDemo.h"

namespace LedWall::Mode {

FancyDemo::FancyDemo(LedMatrix &matrix):
    FancyScript(matrix)
{
    setScript({
        "ExpLet,0,5",
        "ExpLet,0,4",
        "ExpLet,0,3",
        "ExpLet,0,2",
        "ExpLet,0,1",
        "ExpLet,0,2022",
        "Sprinkle",
        "ExpLet,1,2022",
        "ColorRays,1",
        "ColorRays",
        "ExpLet,0,2022",
        "BarsGrow",
        "BarsOpen",
        "ExpLet,1,2022",
        "BarsOpen,1",
        "BarsGrow,1",
        "ExpLet,0,2022",
        "CubeGrow,1",
        "CubeRoll",
        "ColorRays",
        "CircleGrow",
        "ColorRays,1",
        "CircleGrow,1",
        "CubeRoll,1",
        "BarsGrow,2",
        "BarsOpen,2",
        "CircleGrow,1",
        "CircleGrow,1",
        "ExpLet,1,2022",
        "BarsOpen,3",
        "BarsGrow,3",
        "CN",
    });
}

}
