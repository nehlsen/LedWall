#include "TimeFormatter.h"

namespace LedWall {

std::string TimeFormatter::formatSeconds(int secondsToFormat, bool includeColon)
{
    int seconds = secondsToFormat % 60;
    int minutesToFormat = (secondsToFormat - seconds) / 60;
    int minutes = minutesToFormat % 60;
    int hoursToFormat = (minutesToFormat - minutes) / 60;
    int hours = hoursToFormat % 24;
    int days = (hoursToFormat - hours) / 24;

    if (days > 9) {
        return "long";
    }
    if (days >= 1) {
        return std::to_string(days) + "days";
    }
    if (hours >= 1) {
        return formatTwoTimeParts(hours, minutes, includeColon);
    }

    return formatTwoTimeParts(minutes, seconds, includeColon);
}

std::string TimeFormatter::formatTwoTimeParts(int partOne, int partTwo, bool includeColon)
{
    std::string formattedTimeParts;

    formattedTimeParts += (partOne < 10 ? "0" : "");
    formattedTimeParts += std::to_string(partOne);

    formattedTimeParts += (includeColon ? ":" : " ");

    formattedTimeParts += (partTwo < 10 ? "0" : "");
    formattedTimeParts += std::to_string(partTwo);

    return formattedTimeParts;
}

}
