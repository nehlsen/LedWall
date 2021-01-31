#ifndef LEDWALL_HTTP_POWER_H
#define LEDWALL_HTTP_POWER_H

#include <HttpModule.h>
#include "../ModeController.h"

namespace LedWall::http {

class Power: public EBLi::http::module::HttpModule
{
public:
    explicit Power(ModeController *controller);

    [[nodiscard]] std::vector<httpd_uri_t *> getHandlers() override;

private:
    ModeController *m_controller;

    httpd_uri_t m_get_power_uri;
    static esp_err_t getPowerHttpHandler(httpd_req_t *request);
    httpd_uri_t m_post_power_uri;
    static esp_err_t postPowerHttpHandler(httpd_req_t *request);

    static cJSON *createLedPowerInfo(ModeController *controller);
};

}

#endif //LEDWALL_HTTP_POWER_H
