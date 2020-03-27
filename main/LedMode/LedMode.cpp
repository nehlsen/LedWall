#include "LedMode.h"

uint8_t LedMode::matrixWidth = 4;
uint8_t LedMode::matrixHeight = 4;
bool LedMode::serpentineLayout = true;

void LedMode::setup(uint8_t width, uint8_t height, bool serpentine)
{
    matrixWidth = width;
    matrixHeight = height;
    serpentineLayout = serpentine;
}

void LedMode::readOptions(cJSON *root)
{}

bool LedMode::writeOptions(cJSON *root)
{
    return true;
}

uint16_t LedMode::xyToIndex(uint8_t x, uint8_t y)
{
    // WARNING: the following code assumes LEDs have a "safety pixel"!!!
    if (x >= matrixWidth) {
        return -1;
    }

    if (y >= matrixHeight) {
        return -1;
    }

    uint16_t index;

    if (serpentineLayout) {
        if (y & 0x01) {
            // Odd rows run backwards
            uint8_t reverseX = (matrixWidth - 1) - x;
            index = (y * matrixWidth) + reverseX;
        } else {
            // Even rows run forwards
            index = (y * matrixWidth) + x;
        }
    } else {
        index = (y * matrixWidth) + x;
    }

    return index;
}