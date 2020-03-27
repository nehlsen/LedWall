#ifndef LEDWALL_UTILITIES_H
#define LEDWALL_UTILITIES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //LEDWALL_UTILITIES_H
