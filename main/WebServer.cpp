#include "WebServer.h"
#include <cJSON.h>
#include <esp_log.h>
#include "LedController.h"

static const char *WEBSERVER_LOG_TAG = "WebServer";

static esp_err_t system_info_get_handler(httpd_req_t *req)
{
    auto *server = (WebServer*)req->user_ctx;
    if (!server) {
        return ESP_FAIL;
    }

    return server->reqSystemInfo(req);
}

static esp_err_t led_state_get_handler(httpd_req_t *req)
{
    auto *server = (WebServer*)req->user_ctx;
    if (!server) {
        return ESP_FAIL;
    }

    return server->reqLedState(req);
}

WebServer::WebServer(LedController *ledController):
    m_ledController(ledController)
{
}

esp_err_t WebServer::reqSystemInfo(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    cJSON *root = cJSON_CreateObject();
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    cJSON_AddStringToObject(root, "version", IDF_VER);
    cJSON_AddNumberToObject(root, "cores", chip_info.cores);

    const char *sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);

    free((void *)sys_info);
    cJSON_Delete(root);

    return ESP_OK;
}

esp_err_t WebServer::reqLedState(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    cJSON *root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "power", m_ledController->getPower());
    cJSON_AddNumberToObject(root, "mode", m_ledController->getMode());

    const char *sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);

    free((void *)sys_info);
    cJSON_Delete(root);

    return ESP_OK;
}

void WebServer::startServer()
{
    if (m_hdnlServer != nullptr) {
        ESP_LOGE(WEBSERVER_LOG_TAG, "NOT starting WebServer, already running...");
        return;
    }

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(WEBSERVER_LOG_TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&m_hdnlServer, &config) == ESP_OK) {
        registerUriHandlers();
    } else {
        ESP_LOGE(WEBSERVER_LOG_TAG, "Failed to start WebServer");
    }
}

void WebServer::stopServer()
{
    httpd_stop(m_hdnlServer);
}

void WebServer::registerUriHandlers()
{
    ESP_LOGI(WEBSERVER_LOG_TAG, "Registering URI handlers");

    httpd_uri_t system_info_get_uri = {
            .uri = "/api/v1/system/info",
            .method = HTTP_GET,
            .handler = system_info_get_handler,
            .user_ctx = this
    };
    httpd_register_uri_handler(m_hdnlServer, &system_info_get_uri);

    httpd_uri_t led_state_get_uri = {
            .uri = "/api/v1/led/state",
            .method = HTTP_GET,
            .handler = led_state_get_handler,
            .user_ctx = this
    };
    httpd_register_uri_handler(m_hdnlServer, &led_state_get_uri);
}
