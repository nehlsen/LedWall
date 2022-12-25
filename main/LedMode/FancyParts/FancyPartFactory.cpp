#include "FancyPartFactory.h"
#include "Nothing.h"
#include "ExplodingLetters.h"
#include "Sprinkle.h"
#include "ColorRays.h"
#include "BarsGrow.h"
#include "BarsOpen.h"
#include "CubeRoll.h"
#include "CubeGrow.h"
#include "CircleGrow.h"
#include "CN.h"

namespace LedWall::Mode::FancyParts {

FancyPart *FancyPartFactory::create(const std::string &partName, LedMatrix &matrix, PartOptions options, const std::string &arguments)
{
    if (partName == "ExpLet") {
        return new FancyParts::ExplodingLetters(matrix, options, arguments);
    }
    if (partName == "Sprinkle") {
        return new FancyParts::Sprinkle(matrix, options, arguments);
    }
    if (partName == "ColorRays") {
        return new FancyParts::ColorRays(matrix, options, arguments);
    }
    if (partName == "BarsGrow") {
        return new FancyParts::BarsGrow(matrix, options, arguments);
    }
    if (partName == "BarsOpen") {
        return new FancyParts::BarsOpen(matrix, options, arguments);
    }
    if (partName == "CubeRoll") {
        return new FancyParts::CubeRoll(matrix, options, arguments);
    }
    if (partName == "CubeGrow") {
        return new FancyParts::CubeGrow(matrix, options, arguments);
    }
    if (partName == "CircleGrow") {
        return new FancyParts::CircleGrow(matrix, options, arguments);
    }
    if (partName == "CN") {
        return new FancyParts::CN(matrix, options, arguments);
    }
    if (partName == "Nothing") {
        return new FancyParts::Nothing(matrix, options, arguments);
    }

    return new FancyParts::Nothing(matrix, options, arguments);
}

}
