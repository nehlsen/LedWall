#include "LedMode.h"

namespace LedWall {
namespace Mode {

LedMode::LedMode(LedMatrix &matrix):
    m_matrix(matrix)
{}

int LedMode::frameDelay() const
{
    // FIXME fire normally uses 60fps
    return (1000/25) / portTICK_PERIOD_MS;
}

void LedMode::readOptions(cJSON *root)
{}

bool LedMode::writeOptions(cJSON *root)
{
    return true;
}

} // namespace Mode
} // namespace LedWall
