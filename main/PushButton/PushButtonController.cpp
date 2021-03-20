#include "PushButtonController.h"
#include <button.h>
#include <esp_log.h>

namespace LedWall::PushButton {

static void buttonCallback(void *args)
{
    ESP_LOGI("PushButton", "buttonCallback...");

    auto buttonCallbackArgs = static_cast<ButtonCallbackArgs*>(args);
    if (nullptr == buttonCallbackArgs) {
        return;
    }

    ESP_LOGI("PushButton", "buttonCallback -> %d", buttonCallbackArgs->action);
    buttonCallbackArgs->actionMultiplexer->triggerAction(buttonCallbackArgs->action);
}

PushButtonController::PushButtonController(ModeController *modeController):
    m_actionMultiplexer(new ActionMultiplexer(modeController))
{
}

int PushButtonController::addButton(ButtonConfig btn)
{
    ESP_LOGI("PushButtonController", "addButton, gpio: %d, action: %d", btn.gpio, btn.action);

    auto button = new Button(btn.gpio);
    ButtonCallbackArgs callbackArgs = {
        .actionMultiplexer = m_actionMultiplexer,
        .action = btn.action
    };

    m_buttons.push_back(std::pair(button, callbackArgs));

    button->setEventCallback(BUTTON_CB_TAP, &buttonCallback, &m_buttons.back().second);

    return m_buttons.size();
}

}