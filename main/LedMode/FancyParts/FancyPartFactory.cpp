#include "FancyPartFactory.h"
#include "Nothing.h"
#include "ExplodingLetters.h"
#include "Sprinkle.h"

namespace LedWall::Mode::FancyParts {

FancyPart *FancyPartFactory::create(const std::string &partName, LedMatrix &matrix, PartOptions options, const std::string &arguments)
{
    if (partName == "ExpLet") {
        return new FancyParts::ExplodingLetters(matrix, options, arguments);
    }
    if (partName == "Sprinkle") {
        return new FancyParts::Sprinkle(matrix, options, arguments);
    }
    if (partName == "Nothing") {
        return new FancyParts::Nothing(matrix, options, arguments);
    }

    return new FancyParts::Nothing(matrix, options, arguments);
}

}
