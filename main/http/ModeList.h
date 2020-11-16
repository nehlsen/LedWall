#ifndef LEDWALL_HTTP_MODELIST_H
#define LEDWALL_HTTP_MODELIST_H

#include <HttpModule.h>

namespace LedWall::http {

class ModeList: public EBLi::http::module::HttpModule
{
public:
    [[nodiscard]] std::vector<HttpEndpoint> getHttpEndpoints() const override;
};

}

#endif //LEDWALL_HTTP_MODELIST_H
