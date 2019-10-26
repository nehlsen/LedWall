#ifndef MQTT_TCP_MQTT_H
#define MQTT_TCP_MQTT_H

#include "LedController.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "mqtt_client.h"

esp_mqtt_client_handle_t mqtt_app_start(LedController *ledController);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //MQTT_TCP_MQTT_H
