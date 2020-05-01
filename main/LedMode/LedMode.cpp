#include "LedMode.h"

LedMode::LedMode(LedMatrix &matrix):
    m_matrix(matrix)
{}

int LedMode::frameDelay() const
{
    return (1000/25) / portTICK_PERIOD_MS;
}

void LedMode::readOptions(cJSON *root)
{}

bool LedMode::writeOptions(cJSON *root)
{
    return true;
}
