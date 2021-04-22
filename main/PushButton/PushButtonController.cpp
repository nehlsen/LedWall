#include "PushButtonController.h"
#include "../events.h"
#include <button.h>
#include <esp_log.h>

namespace LedWall::PushButton {

static void buttonCallback(void *args)
{
    ESP_LOGI("PushButton", "buttonCallback...");

    auto event = static_cast<int32_t*>(args);
    esp_event_post(LEDWALL_EVENTS, *event, nullptr, 0, portMAX_DELAY);
}

int PushButtonController::addButton(ButtonConfig btn)
{
    ESP_LOGI("PushButtonController", "addButton, gpio: %d, event: %d", btn.gpio, btn.event);

    auto buttonToEvent = std::pair(new Button(btn.gpio), btn.event);
    m_buttons.push_back(buttonToEvent);

    m_buttons.back().first->setEventCallback(BUTTON_CB_TAP, &buttonCallback, &m_buttons.back().second);

    return m_buttons.size();
}

}