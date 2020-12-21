#ifndef LEDWALL_TIMEFORMATTER_H
#define LEDWALL_TIMEFORMATTER_H

#include <string>

namespace LedWall {

class TimeFormatter
{
public:
    static std::string formatSeconds(int secondsToFormat, bool includeColon);

private:
    static std::string formatTwoTimeParts(int partOne, int partTwo, bool includeColon);
};

}

#endif //LEDWALL_TIMEFORMATTER_H
