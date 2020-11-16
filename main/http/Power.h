#ifndef LEDWALL_HTTP_POWER_H
#define LEDWALL_HTTP_POWER_H

#include <HttpModule.h>
#include "../ModeController.h"

namespace LedWall::http {

class Power: public EBLi::http::module::HttpModule
{
public:
    explicit Power(ModeController *controller);

    [[nodiscard]] std::vector<HttpEndpoint> getHttpEndpoints() const override;

private:
    ModeController *m_controller;
};

}

#endif //LEDWALL_HTTP_POWER_H
