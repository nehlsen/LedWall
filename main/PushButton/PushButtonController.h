#ifndef LEDWALL_PUSHBUTTONCONTROLLER_H
#define LEDWALL_PUSHBUTTONCONTROLLER_H

#include <driver/gpio.h>
#include <vector>
#include "ActionMultiplexer.h"
#include "ActionListener.h"

class Button;

namespace LedWall::PushButton {

struct ButtonCallbackArgs {
    ActionMultiplexer *actionMultiplexer = nullptr;
    Action action = ACTION_NONE;
};

class PushButtonController
{
public:
    explicit PushButtonController();

    struct ButtonConfig {
        gpio_num_t gpio;
        Action action;
    };
    int addButton(ButtonConfig btn);

    void addActionListener(ActionListener *actionListener);

private:
    ActionMultiplexer *m_actionMultiplexer;

    std::vector<std::pair<Button*, ButtonCallbackArgs>> m_buttons;
};

}

#endif //LEDWALL_PUSHBUTTONCONTROLLER_H
