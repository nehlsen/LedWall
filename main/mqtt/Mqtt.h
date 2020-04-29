#ifndef LEDWALL_MQTT_H
#define LEDWALL_MQTT_H

#include <string>
#include <mqtt_client.h>

class ModeController;
class ConfigManager;

class Mqtt
{
public:
    Mqtt(ModeController* controller, ConfigManager* configManager);

    void start();
    void stop();

    void onMqttConnected();
    void onMqttData(esp_mqtt_event_handle_t event);
    void onLedWallEvent(int32_t event_id, void* event_data);

protected:
    ModeController* m_controller;
    ConfigManager* m_configManager;

    esp_mqtt_client_handle_t m_client;

    void setupSubscriptions(const std::string &baseTopic);
    void publishState(const std::string &state, const std::string &value);
};

#endif //LEDWALL_MQTT_H
