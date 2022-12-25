#ifndef LEDWALL_FANCYSCRIPT_FANCYPARTFACTORY_H
#define LEDWALL_FANCYSCRIPT_FANCYPARTFACTORY_H

#include "FancyPart.h"

class LedMatrix;

namespace LedWall::Mode::FancyParts {

class FancyPartFactory
{
public:
    static FancyPart* create(const std::string& partName, LedMatrix &matrix, PartOptions options, const std::string& arguments);
};

}

#endif //LEDWALL_FANCYSCRIPT_FANCYPARTFACTORY_H
