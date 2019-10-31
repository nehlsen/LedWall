#ifndef LEDWALL_WEBSERVER_H
#define LEDWALL_WEBSERVER_H

#include <esp_http_server.h>

class LedController;

class WebServer
{
public:
    WebServer(LedController *ledController);

    esp_err_t reqSystemInfo(httpd_req_t *req);
    esp_err_t reqLedState(httpd_req_t *req);

    void startServer();
    void stopServer();

protected:
    LedController *m_ledController;
    httpd_handle_t m_hdnlServer = nullptr;

    void registerUriHandlers();
};

#endif //LEDWALL_WEBSERVER_H
