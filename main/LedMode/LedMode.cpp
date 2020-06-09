#include "LedMode.h"

namespace LedWall {
namespace Mode {

LedMode::LedMode(LedMatrix &matrix):
    m_matrix(matrix)
{}

int LedMode::framesPerSecond() const
{
    return 25;
}

int LedMode::frameDelay() const
{
    return (1000/framesPerSecond()) / portTICK_PERIOD_MS;
}

void LedMode::readOptions(cJSON *root)
{}

bool LedMode::writeOptions(cJSON *root)
{
    return true;
}

} // namespace Mode
} // namespace LedWall
