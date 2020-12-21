#ifndef LEDWALL_TIMEFORMATTER_H
#define LEDWALL_TIMEFORMATTER_H

#include <string>

namespace LedWall {

class TimeFormatter
{
public:
    static std::string formatSeconds(int secondsToFormat, bool includeColon);
};

}

#endif //LEDWALL_TIMEFORMATTER_H
