#include "LedMode.h"

void LedMode::setup(LedMatrix *pMatrix)
{
    matrix = pMatrix;
}

void LedMode::readOptions(cJSON *root)
{}

bool LedMode::writeOptions(cJSON *root)
{
    return true;
}
