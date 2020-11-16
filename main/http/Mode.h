#ifndef LEDWALL_HTTP_MODE_H
#define LEDWALL_HTTP_MODE_H

#include <HttpModule.h>
#include "../ModeController.h"

namespace LedWall::http {

class Mode: public EBLi::http::module::HttpModule
{
public:
    explicit Mode(ModeController *controller);

    [[nodiscard]] std::vector<HttpEndpoint> getHttpEndpoints() const override;

private:
    ModeController *m_controller;
};

}

#endif //LEDWALL_HTTP_MODE_H
