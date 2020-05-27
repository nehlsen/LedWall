#ifndef LEDWALL_CAMERACONFIG_H
#define LEDWALL_CAMERACONFIG_H

#include <esp_camera.h>

#define CAM_PIN_PWDN    32
#define CAM_PIN_RESET   -1
#define CAM_PIN_XCLK    0
#define CAM_PIN_SIOD    26
#define CAM_PIN_SIOC    27
#define CAM_PIN_D7      35
#define CAM_PIN_D6      34
#define CAM_PIN_D5      39
#define CAM_PIN_D4      36
#define CAM_PIN_D3      21
#define CAM_PIN_D2      19
#define CAM_PIN_D1      18
#define CAM_PIN_D0       5
#define CAM_PIN_VSYNC   25
#define CAM_PIN_HREF    23
#define CAM_PIN_PCLK    22

static camera_config_t camera_config = {
        .pin_pwdn  = CAM_PIN_PWDN,
        .pin_reset = CAM_PIN_RESET,
        .pin_xclk = CAM_PIN_XCLK,
        .pin_sscb_sda = CAM_PIN_SIOD,
        .pin_sscb_scl = CAM_PIN_SIOC,

        .pin_d7 = CAM_PIN_D7,
        .pin_d6 = CAM_PIN_D6,
        .pin_d5 = CAM_PIN_D5,
        .pin_d4 = CAM_PIN_D4,
        .pin_d3 = CAM_PIN_D3,
        .pin_d2 = CAM_PIN_D2,
        .pin_d1 = CAM_PIN_D1,
        .pin_d0 = CAM_PIN_D0,
        .pin_vsync = CAM_PIN_VSYNC,
        .pin_href = CAM_PIN_HREF,
        .pin_pclk = CAM_PIN_PCLK,

        //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
        .xclk_freq_hz = 20000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,

//        .pixel_format = PIXFORMAT_JPEG,//YUV422,GRAYSCALE,RGB565,JPEG
        .pixel_format = PIXFORMAT_GRAYSCALE,//YUV422,GRAYSCALE,RGB565,JPEG
//        .frame_size = FRAMESIZE_UXGA,//QQVGA-QXGA Do not use sizes above QVGA when not JPEG
        .frame_size = FRAMESIZE_QQVGA,//QQVGA-QXGA Do not use sizes above QVGA when not JPEG

        .jpeg_quality = 12, //0-63 lower number means higher quality
        .fb_count = 1 //if more than one, i2s runs in continuous mode. Use only with JPEG
};

#endif //LEDWALL_CAMERACONFIG_H
