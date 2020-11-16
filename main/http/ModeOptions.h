#ifndef LEDWALL_HTTP_MODEOPTIONS_H
#define LEDWALL_HTTP_MODEOPTIONS_H

#include <HttpModule.h>
#include "../ModeController.h"

namespace LedWall::http {

class ModeOptions: public EBLi::http::module::HttpModule
{
public:
    explicit ModeOptions(ModeController *controller);

    [[nodiscard]] std::vector<HttpEndpoint> getHttpEndpoints() const override;

private:
    ModeController *m_controller;
};

}

#endif //LEDWALL_HTTP_MODEOPTIONS_H
