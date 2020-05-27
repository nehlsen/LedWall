#ifndef LEDWALL_CAMERA_H
#define LEDWALL_CAMERA_H

/**
 * adaptation of LEDWallCamera by bitluni
 * https://github.com/bitluni/LEDWallCamera
 */

#include "LedMode.h"
#include "esp_camera.h"
#include <GfxPrimitive.h>

class Camera : public LedMode
{
public:
    explicit Camera(LedMatrix &matrix);

    void update() override;

protected:
    GfxPrimitive m_image;
    uint8_t m_lastUpdate = 99;
    void updateImage();
    GfxPrimitive downSample(camera_fb_t *fb);

    bool cameraInit();
    void setCameraParams();
};

#endif //LEDWALL_CAMERA_H
