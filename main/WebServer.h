#ifndef LEDWALL_WEBSERVER_H
#define LEDWALL_WEBSERVER_H

#include <esp_http_server.h>
#include <functional>

class cJSON;

namespace EBLi {
class OtaUpdater;
}

namespace LedWall {
class ModeController;
class ConfigManager;

#define READ_BUFFER_SIZE 1024 * 5

class WebServer
{
public:
    WebServer(ModeController *controller, ConfigManager *configManager, EBLi::OtaUpdater *otaUpdater);

    esp_err_t getSystemInfo(httpd_req_t *req);

    esp_err_t postSystemReboot(httpd_req_t *req);

    esp_err_t getLedPower(httpd_req_t *req);
    esp_err_t postLedPower(httpd_req_t *req);

    esp_err_t getLedMode(httpd_req_t *req);
    esp_err_t postLedMode(httpd_req_t *req);
    esp_err_t getLedModes(httpd_req_t *req);

    esp_err_t postModeOptions(httpd_req_t *req);
    esp_err_t deleteModeOptions(httpd_req_t *req);

    esp_err_t getConfig(httpd_req_t *req);
    esp_err_t postConfig(httpd_req_t *req);

    esp_err_t postOta(httpd_req_t *req);

    esp_err_t getFile(httpd_req_t *req);

    void startServer();
    void stopServer();

protected:
    ModeController *m_controller;
    ConfigManager *m_configManager;
    EBLi::OtaUpdater *m_otaUpdater;

    httpd_handle_t m_hdnlServer = nullptr;
    char m_readBuffer[READ_BUFFER_SIZE];

    void registerUriHandlers();

    esp_err_t jsonResponse(cJSON *root, httpd_req_t *req, const char* status = "200");
    esp_err_t handlePost(httpd_req_t *req, const std::function<bool(cJSON *request, cJSON **response)>& jsonHandler);

    cJSON *createLedPowerData();
    cJSON *createLedModeData();
    cJSON *createConfigData();
};

} // namespace LedWall

#endif //LEDWALL_WEBSERVER_H
