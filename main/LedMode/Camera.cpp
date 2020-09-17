#include "Camera.h"
#include <esp_camera.h>
#include <cJSON.h>
#include "CameraConfig.h"

namespace LedWall {
namespace Mode {

#define CAM_LOG_TAG "Camera"

Camera::Camera(LedMatrix &matrix) : LedMode(matrix)
{
    cameraInit();
    setCameraParams();
}

void Camera::readOptions(cJSON *root)
{
    sensor_t *sensor = esp_camera_sensor_get();

    cJSON_AddNumberToObject(root, "brightness", sensor->status.brightness);
    cJSON_AddNumberToObject(root, "contrast", sensor->status.contrast);
    cJSON_AddNumberToObject(root, "saturation", sensor->status.saturation);
}

bool Camera::writeOptions(cJSON *root)
{
    bool changed = false;

    cJSON *objectItem = cJSON_GetObjectItem(root, "brightness");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        int newValue = constrain(objectItem->valueint, -2, 2);
        sensor_t *sensor = esp_camera_sensor_get();
        sensor->set_brightness(sensor, newValue);
        changed = true;
    }

    objectItem = cJSON_GetObjectItem(root, "contrast");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        int newValue = constrain(objectItem->valueint, -2, 2);
        sensor_t *sensor = esp_camera_sensor_get();
        sensor->set_contrast(sensor, newValue);
        changed = true;
    }

    objectItem = cJSON_GetObjectItem(root, "saturation");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        int newValue = constrain(objectItem->valueint, -2, 2);
        sensor_t *sensor = esp_camera_sensor_get();
        sensor->set_saturation(sensor, newValue);
        changed = true;
    }

    return changed;
}

bool Camera::update()
{
//    uint8_t currentSeconds = (millis() / 1000) % 60;

//    if(m_lastUpdate != currentSeconds) {
//        m_lastUpdate = currentSeconds;

        updateImage();
//        m_image.render(m_matrix);
        return true;
//    }

//    return false;
}

void Camera::updateImage()
{
    camera_fb_t *fb = esp_camera_fb_get();
    if (fb) {
        /*m_image = */downSample(fb);
        esp_camera_fb_return(fb);
    }
}

GfxPrimitive Camera::downSample(camera_fb_t *fb)
{
    const size_t vertical_scaling = fb->height / m_matrix.getHeight();
    const size_t horizontal_scaling = fb->width / m_matrix.getWidth();
    const int scaling = std::min(horizontal_scaling, vertical_scaling);

    GfxPrimitive buffer;
    if (fb->format == PIXFORMAT_GRAYSCALE) {
        // 1bpp
        for (int x = 0; x < m_matrix.getWidth(); ++x) {
            for (int y = 0; y < m_matrix.getHeight(); ++y) {
                // each camera frame is 160 in width...
                // since my matrix is wider than high... rotate!
                // take every 7th pixel
                int fb_x = x * scaling;
                int fb_y = y * scaling;
                int fb_index = fb_y * fb->width + fb_x;

//                ESP_LOGI("Camera::downSample", "x/y:%d,%d -> fb x/y:%d,%d -> fb-idx:%d/%d", x, y, fb_x, fb_y, fb_index, fb->len);

                if (fb_index < fb->len) {
                    uint8_t value = fb->buf[fb_index];
//                    buffer.merge(Pixel(x, y, CRGB(value, value, value)));
//                    m_matrix.pixel(x, y) = CRGB(value, value, value);
                    const uint8_t msbCutoffMask = 0b00111111;
                    m_matrix.pixel(x, y).setHSV(0, 0, value&msbCutoffMask);
                }
            }
        }
    }

    return buffer;
}

bool Camera::cameraInit()
{
    const int maxInitTries = 10;
    int tries = 0;
    esp_err_t err;
    do {
        ESP_LOGI(CAM_LOG_TAG, "Trying to initialize camera (try %d/%d)...", tries+1, maxInitTries);

        //initialize the camera
        err = esp_camera_init(&camera_config);
        if (err != ESP_OK) {
            ESP_LOGW(CAM_LOG_TAG, "Camera Init Failed. Retrying in 1sec...");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        tries++;
    } while(err != ESP_OK && tries < maxInitTries);

    if(err == ESP_OK) {
        ESP_LOGI(CAM_LOG_TAG, "Camera Init Success");
    } else {
        ESP_LOGE(CAM_LOG_TAG, "Camera init failed after %d tries. Restarting in 3 Seconds...", maxInitTries);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        esp_restart();
    }

    return err == ESP_OK;
}

void Camera::setCameraParams()
{
    sensor_t *sensor = esp_camera_sensor_get();
    sensor->set_brightness(sensor, -2);
    sensor->set_contrast(sensor, 2);
    sensor->set_saturation(sensor, 0);

    //0: auto, 1: sun, 2: cloud, 3: indoors
    sensor->set_wb_mode(sensor, 0);

    //sensor->set_exposure_ctrl(sensor, 1);
    //sensor->set_aec_value(sensor, -2);
    //sensor->set_ae_level(sensor, 100);
    //sensor->set_gain_ctrl(sensor, 100);


/*
*   some other prameters.. good luck
    int  (*set_sharpness)       (sensor_t *sensor, int level);
    int  (*set_denoise)         (sensor_t *sensor, int level);
    int  (*set_gainceiling)     (sensor_t *sensor, gainceiling_t gainceiling);
    int  (*set_quality)         (sensor_t *sensor, int quality);
    int  (*set_colorbar)        (sensor_t *sensor, int enable);
    int  (*set_whitebal)        (sensor_t *sensor, int enable);
    int  (*set_gain_ctrl)       (sensor_t *sensor, int enable);
    int  (*set_exposure_ctrl)   (sensor_t *sensor, int enable);
    int  (*set_hmirror)         (sensor_t *sensor, int enable);
    int  (*set_vflip)           (sensor_t *sensor, int enable);
    int  (*set_aec2)            (sensor_t *sensor, int enable);
    int  (*set_awb_gain)        (sensor_t *sensor, int enable);
    int  (*set_agc_gain)        (sensor_t *sensor, int gain);
    int  (*set_aec_value)       (sensor_t *sensor, int gain);
    int  (*set_special_effect)  (sensor_t *sensor, int effect);
    int  (*set_wb_mode)         (sensor_t *sensor, int mode);
    int  (*set_ae_level)        (sensor_t *sensor, int level);
    int  (*set_dcw)             (sensor_t *sensor, int enable);
    int  (*set_bpc)             (sensor_t *sensor, int enable);
    int  (*set_wpc)             (sensor_t *sensor, int enable);
    int  (*set_raw_gma)         (sensor_t *sensor, int enable);
    int  (*set_lenc)            (sensor_t *sensor, int enable);  */
}

} // namespace Mode
} // namespace LedWall
