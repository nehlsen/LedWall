#include "TimeFormatter.h"

namespace LedWall {

std::string TimeFormatter::formatSeconds(int secondsToFormat, bool includeColon)
{
    std::string formattedSeconds;

    int seconds = secondsToFormat % 60;
    int minutes = (secondsToFormat-seconds) / 60;
    // FIXME if minutes > 99

    formattedSeconds += (minutes < 10 ? "0" : "");
    formattedSeconds += std::to_string(minutes);

    formattedSeconds += (includeColon ? ":" : " ");

    formattedSeconds += (seconds < 10 ? "0" : "");
    formattedSeconds += std::to_string(seconds);

    return formattedSeconds;
}

}
