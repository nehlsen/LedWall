#include "ModeTime.h"
#include <esp_log.h>

namespace LedWall {
namespace Mode {

ModeTime::ModeTime(LedMatrix &matrix): ModeText(matrix) {}

bool ModeTime::update()
{
    const uint16_t delay = 1000;
    int64_t currentTime = esp_timer_get_time() / 1000;
    if ((currentTime - m_lastTimeUpdate < delay) && m_lastTimeUpdate > 0) {
        return false;
    }
    m_lastTimeUpdate = currentTime;

    static bool isOdd = false;
    isOdd = !isOdd;

    time_t now;
    char strftime_buf[64];
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), (isOdd ? "%H %M" : "%H:%M"), &timeinfo);

    setScrollMode(ScrollNone);
    setText(strftime_buf);

    return ModeText::update();
}

}
}
