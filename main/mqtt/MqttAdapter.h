#ifndef LEDWALL_MQTTADAPTER_H
#define LEDWALL_MQTTADAPTER_H

#include <string>

namespace EBLi::mqtt {
class MqttPublisher;
}

namespace LedWall {

class ModeController;
class PresetChanger;

class MqttAdapter
{
public:
    explicit MqttAdapter(ModeController *controller, PresetChanger *presetChanger);

    void onLedWallEvent(int32_t event_id, void* event_data);

protected:
    ModeController* m_controller;
    PresetChanger *m_presetChanger;

    EBLi::mqtt::MqttPublisher *m_publisherPower;
    EBLi::mqtt::MqttPublisher *m_publisherBrightness;
    EBLi::mqtt::MqttPublisher *m_publisherModeIndex;
    EBLi::mqtt::MqttPublisher *m_publisherModeName;
    EBLi::mqtt::MqttPublisher *m_publisherModeOptions;

    void publishMode();
    void publishModeOptions();

    void setupPublishers();
    void setupSubscribers();
};

}

#endif //LEDWALL_MQTTADAPTER_H
