#include "Sensors.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Mqtt.h>
#include <MqttPublisher.h>

#if defined(CONFIG_LEDWALL_SENSOR_AHT10)
#include <AHT10.h>
#endif
#if defined(CONFIG_ENABLE_EBLI_CONFIG_MANAGER)
#include <ConfigManager.h>
#endif

#define PUBLISH_DELAY 15000

namespace LedWall {
namespace Sensors {

class SensorsP
{
public:
    SensorsP();

    void publishAll();

private:
#if defined(CONFIG_LEDWALL_SENSOR_AHT10)
    AHT10 *m_aht10 = nullptr;
    EBLi::MqttPublisher *m_aht10TemperaturePublisher;
    EBLi::MqttPublisher *m_aht10HumidityPublisher;
#endif
};

[[noreturn]] void sensors_task_task(void *pvParameter)
{
    auto sp = static_cast<SensorsP*>(pvParameter);

    while (true) {
        if (sp) sp->publishAll();
        vTaskDelay(PUBLISH_DELAY / portTICK_PERIOD_MS);
    }
}

SensorsP::SensorsP()
{
#if defined(CONFIG_LEDWALL_SENSOR_AHT10)
#if defined(CONFIG_ENABLE_EBLI_CONFIG_MANAGER)
    auto configManager = EBLi::ConfigManager::instance();
    int i2cPinSda = configManager->property("aht10_i2c_sda")
            ->setDefaultValue(CONFIG_AHT10_PIN_SDA)
            ->getValue<int>();
    int i2cPinScl = configManager->property("aht10_i2c_scl")
            ->setDefaultValue(CONFIG_AHT10_PIN_SCL)
            ->getValue<int>();
    m_aht10 = new AHT10(i2cPinSda, i2cPinScl);
#else
    m_aht10 = new AHT10;
#endif
    auto mqtt = EBLi::Mqtt::instance();
    m_aht10TemperaturePublisher = mqtt->createPublisher("aht10/temperature");
    m_aht10HumidityPublisher = mqtt->createPublisher("aht10/humidity");
#endif

    xTaskCreate(
            &sensors_task_task,
            "sensors_task_task",
            2*1024,
            this,
            tskIDLE_PRIORITY,
            nullptr
    );
}

void SensorsP::publishAll()
{
#if defined(CONFIG_LEDWALL_SENSOR_AHT10)
    m_aht10->readRawData();
    m_aht10TemperaturePublisher->publishValue(std::to_string(m_aht10->getTemperature()));
    m_aht10HumidityPublisher->publishValue(std::to_string(m_aht10->getHumidity()));
#endif
}

/**********************************************************************************************************************/

Sensors::Sensors():
    m_p(new SensorsP)
{}

}
}
