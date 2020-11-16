#ifndef LEDWALL_HTTP_PRESET_H
#define LEDWALL_HTTP_PRESET_H

#include <HttpModule.h>
#include "../ModeController.h"

namespace LedWall::http {

class Preset: public EBLi::http::module::HttpModule
{
public:
    explicit Preset(ModeController *controller);

    [[nodiscard]] std::vector<HttpEndpoint> getHttpEndpoints() const override;

private:
    ModeController *m_controller;
};

}

#endif //LEDWALL_HTTP_PRESET_H
