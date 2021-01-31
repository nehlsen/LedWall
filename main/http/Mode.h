#ifndef LEDWALL_HTTP_MODE_H
#define LEDWALL_HTTP_MODE_H

#include <HttpModule.h>
#include "../ModeController.h"

namespace LedWall::http {

class Mode: public EBLi::http::module::HttpModule
{
public:
    explicit Mode(ModeController *controller);

    [[nodiscard]] std::vector<httpd_uri_t *> getHandlers() override;

private:
    ModeController *m_controller;

    httpd_uri_t m_get_mode_uri;
    static esp_err_t getModeHttpHandler(httpd_req_t *request);
    httpd_uri_t m_post_mode_uri;
    static esp_err_t postModeHttpHandler(httpd_req_t *request);

    static cJSON *createLedModeData(ModeController *controller);
};

}

#endif //LEDWALL_HTTP_MODE_H
