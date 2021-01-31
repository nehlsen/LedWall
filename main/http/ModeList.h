#ifndef LEDWALL_HTTP_MODELIST_H
#define LEDWALL_HTTP_MODELIST_H

#include <HttpModule.h>

namespace LedWall::http {

class ModeList: public EBLi::http::module::HttpModule
{
public:
    ModeList();

    [[nodiscard]] std::vector<httpd_uri_t *> getHandlers() override;

private:
    httpd_uri_t m_get_mode_list_uri;
    static esp_err_t getModeListHttpHandler(httpd_req_t *request);
};

}

#endif //LEDWALL_HTTP_MODELIST_H
