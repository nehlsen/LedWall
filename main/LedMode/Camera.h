#ifndef LEDWALL_CAMERA_H
#define LEDWALL_CAMERA_H

/**
 * adaptation of LEDWallCamera by bitluni
 * https://github.com/bitluni/LEDWallCamera
 */

#include "LedMode.h"

class Camera : public LedMode
{
public:
    using LedMode::LedMode;

    void update() override;
};

#endif //LEDWALL_CAMERA_H
