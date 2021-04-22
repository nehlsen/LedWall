#ifndef LEDWALL_PUSHBUTTONCONTROLLER_H
#define LEDWALL_PUSHBUTTONCONTROLLER_H

#include <driver/gpio.h>
#include <vector>

class Button;

namespace LedWall::PushButton {

class PushButtonController
{
public:
    struct ButtonConfig {
        gpio_num_t gpio;
        int32_t event;
    };
    int addButton(ButtonConfig btn);

private:
    std::vector<std::pair<Button*, int32_t>> m_buttons;
};

}

#endif //LEDWALL_PUSHBUTTONCONTROLLER_H
