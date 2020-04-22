#include "ModeController.h"
static ModeController *ledController;

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include "esp_system.h"
#include "tcpip_adapter.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "mqtt.h"

static const char *MQTT_LOG_TAG = "MQTT_CONNECTOR";

esp_mqtt_client_handle_t mqtt_client;
static const char *MQTT_NAME = "ledwall";

/*
inbound / subscribe
e.g. "/ledwall/cmd/power"

outbound / publish
e.g. "/ledwall/state/power"
*/

void publish_status()
{
    char topic[30];
    char topicValue[2];

    sprintf(topic, "/%s/state/power", MQTT_NAME);
    sprintf(topicValue, "%d", ledController->getPower());
    esp_mqtt_client_publish(mqtt_client, topic, topicValue, 0, 1, 0);

    sprintf(topic, "/%s/state/mode", MQTT_NAME);
    sprintf(topicValue, "%d", ledController->getModeIndex());
    esp_mqtt_client_publish(mqtt_client, topic, topicValue, 0, 1, 0);
}

void setup_subscriptions()
{
    char topic[30];

    sprintf(topic, "/%s/cmd/power", MQTT_NAME);
    esp_mqtt_client_subscribe(mqtt_client, topic, 0);
    ESP_LOGI(MQTT_LOG_TAG, "sent subscribe successful: \"%s\"", topic);

    sprintf(topic, "/%s/cmd/mode", MQTT_NAME);
    esp_mqtt_client_subscribe(mqtt_client, topic, 0);
    ESP_LOGI(MQTT_LOG_TAG, "sent subscribe successful: \"%s\"", topic);
}

void handleMqttData(esp_mqtt_event_handle_t event)
{
//    printf("TOPIC=%.*s (%d)\r\n", event->topic_len, event->topic, event->topic_len);
//    printf("DATA=%.*s (%d)\r\n", event->data_len, event->data, event->data_len);

    if (nullptr != strstr(event->topic, "/power") && event->data_len >= 1) {
        ledController->setPower(event->data[0] == '1');
    }
    if (nullptr != strstr(event->topic, "/mode") && event->data_len >= 1) {
        ledController->setModeIndex(atoi(event->data));
    }
}

esp_err_t log_mqtt_event(esp_mqtt_event_handle_t event)
{
//    esp_mqtt_client_handle_t client = event->client;
//    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_BEFORE_CONNECT:
            ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_BEFORE_CONNECT");
            break;
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_CONNECTED");
//            msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
//            ESP_LOGI(MQTT_LOG_TAG, "sent publish successful, msg_id=%d", msg_id);
//
//            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
//            ESP_LOGI(MQTT_LOG_TAG, "sent subscribe successful, msg_id=%d", msg_id);
//
//            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
//            ESP_LOGI(MQTT_LOG_TAG, "sent subscribe successful, msg_id=%d", msg_id);
//
//            msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
//            ESP_LOGI(MQTT_LOG_TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
//            subscibe_stuff(event->client);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
//            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
//            ESP_LOGI(MQTT_LOG_TAG, "sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_DATA");
//            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
//            printf("DATA=%.*s\r\n", event->data_len, event->data);
//            handleMqttData(event);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(MQTT_LOG_TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(MQTT_LOG_TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(MQTT_LOG_TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    log_mqtt_event((esp_mqtt_event_handle_t)event_data);

    if (event_id == MQTT_EVENT_CONNECTED) {
        setup_subscriptions();
    } else if (event_id == MQTT_EVENT_DATA) {
        handleMqttData((esp_mqtt_event_handle_t)event_data);
    }
}

esp_mqtt_client_handle_t mqtt_app_start(ModeController *controller)
{
    ledController = controller;
    ledController->setChangeHandler(&publish_status);

    esp_mqtt_client_config_t mqtt_cfg = {
            .uri = CONFIG_BROKER_URL,
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, mqtt_event_handler, mqtt_client);
    esp_mqtt_client_start(mqtt_client);

    return mqtt_client;
}

#ifdef __cplusplus
} // extern "C"
#endif
