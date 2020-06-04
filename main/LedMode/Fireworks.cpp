#include "Fireworks.h"

#include <cJSON.h>
#define MAX_INT_VALUE 65536

namespace LedWall {
namespace Mode {

void Fireworks::update()
{
    m_matrix.fade(m_fadeRate);

    if (random8() > m_sparkRate) {
        return;
    }

    uint8_t randomHue = random8();
    uint8_t randomX = random8(m_matrix.getWidth());
    uint8_t randomY = random8(m_matrix.getHeight());

    m_matrix.pixel(randomX, randomY).setHSV(randomHue, 255, 255);
}

void Fireworks::readOptions(cJSON *root)
{
    cJSON_AddNumberToObject(root, "fadeRate", m_fadeRate);
    cJSON_AddNumberToObject(root, "sparkRate", m_sparkRate);
}

bool Fireworks::writeOptions(cJSON *root)
{
    cJSON *const requestedFadeRate = cJSON_GetObjectItem(root, "fadeRate");
    if (requestedFadeRate) {
        int rate = requestedFadeRate->valueint;
        m_fadeRate = constrain(rate, 0, 255);
    }

    cJSON *const requestedSparkRate = cJSON_GetObjectItem(root, "sparkRate");
    if (requestedSparkRate) {
        int rate = requestedSparkRate->valueint;
        m_sparkRate = constrain(rate, 0, 255);
    }

    return requestedFadeRate || requestedSparkRate;
}

} // namespace Mode
} // namespace LedWall
