#ifndef LEDWALL_HTTP_MODEOPTIONS_H
#define LEDWALL_HTTP_MODEOPTIONS_H

#include <HttpModule.h>
#include "../ModeController.h"

namespace LedWall::http {

class ModeOptions: public EBLi::http::module::HttpModule
{
public:
    explicit ModeOptions(ModeController *controller);

    [[nodiscard]] std::vector<httpd_uri_t *> getHandlers() override;

private:
    ModeController *m_controller;

    httpd_uri_t m_get_mode_options_uri;
    static esp_err_t getModeOptionsHttpHandler(httpd_req_t *request);
    httpd_uri_t m_post_mode_options_uri;
    static esp_err_t postModeOptionsHttpHandler(httpd_req_t *request);
    httpd_uri_t m_delete_mode_options_uri;
    static esp_err_t deleteModeOptionsHttpHandler(httpd_req_t *request);
};

}

#endif //LEDWALL_HTTP_MODEOPTIONS_H
