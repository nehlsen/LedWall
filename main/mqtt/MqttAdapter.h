#ifndef LEDWALL_MQTTADAPTER_H
#define LEDWALL_MQTTADAPTER_H

#include <string>

namespace EBLi {
class MqttPublisher;
}

namespace LedWall {

class ModeController;

class MqttAdapter
{
public:
    MqttAdapter(ModeController* controller);

    void onLedWallEvent(int32_t event_id, void* event_data);

protected:
    ModeController* m_controller;

    EBLi::MqttPublisher *m_publisherPower;
    EBLi::MqttPublisher *m_publisherBrightness;
    EBLi::MqttPublisher *m_publisherModeIndex;
    EBLi::MqttPublisher *m_publisherModeName;
    EBLi::MqttPublisher *m_publisherModeOptions;

    void publishMode();
    void publishModeOptions();

    void setupPublishers();
    void setupSubscribers();
};

}

#endif //LEDWALL_MQTTADAPTER_H
