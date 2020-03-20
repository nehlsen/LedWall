#ifndef LEDWALL_UTILITIES_H
#define LEDWALL_UTILITIES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

/**
 * map x/y coordinates to led-string index
 * XY Code and safety pixel code from
 * https://github.com/FastLED/FastLED/blob/master/examples/XYMatrix/XYMatrix.ino
 */
uint16_t xyToIndex(uint8_t x, uint8_t y);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //LEDWALL_UTILITIES_H
