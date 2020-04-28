#include "LedMode.h"

LedMode::LedMode(LedMatrix &matrix):
    m_matrix(matrix)
{}

void LedMode::readOptions(cJSON *root)
{}

bool LedMode::writeOptions(cJSON *root)
{
    return true;
}
