#ifndef LEDWALL_WEBSERVER_H
#define LEDWALL_WEBSERVER_H

#include <esp_http_server.h>
class LedController;

#define READ_BUFFER_SIZE 1024 * 5

class WebServer
{
public:
    WebServer(LedController *ledController);

    esp_err_t getSystemInfo(httpd_req_t *req);
    esp_err_t getLedState(httpd_req_t *req);
    esp_err_t getLedPower(httpd_req_t *req);
    esp_err_t postLedPower(httpd_req_t *req);

    void startServer();
    void stopServer();

protected:
    LedController *m_ledController;
    httpd_handle_t m_hdnlServer = nullptr;
    char m_readBuffer[READ_BUFFER_SIZE];

    void registerUriHandlers();
};

#endif //LEDWALL_WEBSERVER_H
