#include "WebServer.h"
#include <cJSON.h>
#include <esp_log.h>
#include <fcntl.h>
#include "esp_vfs.h"
#include "LedController.h"
#include "ConfigManager.h"
#include "LedMode/LedModes.h"

#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)

static const char *WEBSERVER_LOG_TAG = "WebServer";

#define CREATE_FUNCTION_TO_METHOD(fnName, handler) \
    static esp_err_t fnName(httpd_req_t *req) \
    { \
        auto *server = (WebServer*)req->user_ctx; \
        if (!server) { \
            return ESP_FAIL; \
        } \
        return server->handler(req); \
    }

CREATE_FUNCTION_TO_METHOD(system_info_get_handler, getSystemInfo);
CREATE_FUNCTION_TO_METHOD(led_state_get_handler, getLedState);
CREATE_FUNCTION_TO_METHOD(led_power_get_handler, getLedPower);
CREATE_FUNCTION_TO_METHOD(led_power_post_handler, postLedPower);
CREATE_FUNCTION_TO_METHOD(led_mode_get_handler, getLedMode);
CREATE_FUNCTION_TO_METHOD(led_mode_post_handler, postLedMode);
CREATE_FUNCTION_TO_METHOD(led_modes_get_handler, getLedModes);
CREATE_FUNCTION_TO_METHOD(config_get_handler, getConfig);
CREATE_FUNCTION_TO_METHOD(config_post_handler, postConfig);
CREATE_FUNCTION_TO_METHOD(file_get_handler, getFile);

WebServer::WebServer(LedController *ledController, ConfigManager *configManager) :
    m_ledController(ledController),
    m_configManager(configManager)
{}

esp_err_t WebServer::getSystemInfo(httpd_req_t *req)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "version", IDF_VER);

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    cJSON_AddNumberToObject(root, "cores", chip_info.cores);

    cJSON_AddNumberToObject(root, "restarts", m_configManager->getRestartCounter());

    return jsonResponse(root, req);
}

esp_err_t WebServer::getLedState(httpd_req_t *req)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "power", m_ledController->getPower());
    cJSON_AddNumberToObject(root, "mode-index", m_ledController->getModeIndex());

    return jsonResponse(root, req);
}

esp_err_t WebServer::getLedPower(httpd_req_t *req)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "power", m_ledController->getPower());

    return jsonResponse(root, req);
}

esp_err_t WebServer::postLedPower(httpd_req_t *req)
{
    return handlePost(req, [=](cJSON *root) -> bool {
        cJSON *const powerObj = cJSON_GetObjectItem(root, "power");
        if (!powerObj) {
            return false;
        }

        int power = powerObj->valueint;
        m_ledController->setPower(power);
        return true;
    });
}

esp_err_t WebServer::getLedMode(httpd_req_t *req)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "index", m_ledController->getModeIndex());
    cJSON_AddStringToObject(root, "name", LedModes.at(m_ledController->getModeIndex()).name);

    return jsonResponse(root, req);
}

esp_err_t WebServer::postLedMode(httpd_req_t *req)
{
    return handlePost(req, [=](cJSON *root) -> bool {
        cJSON *const modeObj = cJSON_GetObjectItem(root, "index");
        if (!modeObj) {
            return false;
        }

        int mode = modeObj->valueint;
        return m_ledController->setModeIndex(mode);
    });
}

esp_err_t WebServer::getLedModes(httpd_req_t *req)
{
    cJSON *root = cJSON_CreateObject();
    cJSON *modeList = cJSON_AddArrayToObject(root, "modes");
    for (int idx = 0; idx < LedModes.size(); ++idx) {
        cJSON *modeDescription = cJSON_CreateObject();
        cJSON_AddNumberToObject(modeDescription, "index", idx);
        cJSON_AddStringToObject(modeDescription, "name", LedModes.at(idx).name);
        cJSON_AddItemToArray(modeList, modeDescription);
    }

    return jsonResponse(root, req);
}

esp_err_t WebServer::getConfig(httpd_req_t *req)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "PowerOnResetMode", m_configManager->getPowerOnResetMode());
    cJSON_AddNumberToObject(root, "LedModeAutoRestore", m_configManager->getLedModeAutoRestore());

    return jsonResponse(root, req);
}

esp_err_t WebServer::postConfig(httpd_req_t *req)
{
    return handlePost(req, [=](cJSON *root) -> bool {
        cJSON *const powerOnResetMode = cJSON_GetObjectItem(root, "PowerOnResetMode");
        if (powerOnResetMode) {
            int mode = powerOnResetMode->valueint;
            m_configManager->setPowerOnResetMode((ConfigManager::AutoPowerOn)mode);
        }

        cJSON *const ledModeAutoRestore = cJSON_GetObjectItem(root, "LedModeAutoRestore");
        if (ledModeAutoRestore) {
            int mode = ledModeAutoRestore->valueint;
            m_configManager->setLedModeAutoRestore(mode);
        }

        return powerOnResetMode && ledModeAutoRestore;
    });
}

esp_err_t WebServer::getFile(httpd_req_t *req)
{
    char filepath[FILE_PATH_MAX];

    if (req->uri[strlen(req->uri) - 1] == '/') {
        strlcat(filepath, "/index.html", sizeof(filepath));
    } else {
        strlcat(filepath, req->uri, sizeof(filepath));
    }
    int fd = open(filepath, O_RDONLY, 0);
    if (fd == -1) {
        ESP_LOGE(WEBSERVER_LOG_TAG, "Failed to open file : %s", filepath);
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File not found");
        return ESP_FAIL;
    }

    auto isFileExtension = [filepath](const char *ext)->bool {
        return strcmp(&filepath[strlen(filepath) - strlen(ext)], ext) == 0;
    };

    const char *type = "text/plain";
    if (isFileExtension(".html")) {
        type = "text/html";
    } else if (isFileExtension(".js")) {
        type = "application/javascript";
    } else if (isFileExtension(".css")) {
        type = "text/css";
    } else if (isFileExtension(".png")) {
        type = "image/png";
    } else if (isFileExtension(".ico")) {
        type = "image/x-icon";
    } else if (isFileExtension(".svg")) {
        type = "text/xml";
    }
    httpd_resp_set_type(req, type);

    char *chunk = m_readBuffer;
    ssize_t read_bytes;
    do {
        /* Read file in chunks into the scratch buffer */
        read_bytes = read(fd, chunk, READ_BUFFER_SIZE);
        if (read_bytes == -1) {
            ESP_LOGE(WEBSERVER_LOG_TAG, "Failed to read file : %s", filepath);
        } else if (read_bytes > 0) {
            /* Send the buffer contents as HTTP response chunk */
            if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK) {
                close(fd);
                ESP_LOGE(WEBSERVER_LOG_TAG, "File sending failed!");
                /* Abort sending file */
                httpd_resp_sendstr_chunk(req, nullptr);
                /* Respond with 500 Internal Server Error */
                httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
                return ESP_FAIL;
            }
        }
    } while (read_bytes > 0);
    /* Close file after sending complete */
    close(fd);
    ESP_LOGI(WEBSERVER_LOG_TAG, "File sending complete");
    /* Respond with an empty chunk to signal HTTP response completion */
    httpd_resp_send_chunk(req, nullptr, 0);
    return ESP_OK;
}

void WebServer::startServer()
{
    if (m_hdnlServer != nullptr) {
        ESP_LOGE(WEBSERVER_LOG_TAG, "NOT starting WebServer, already running...");
        return;
    }

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 10;
    config.uri_match_fn = httpd_uri_match_wildcard;

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

    httpd_uri_t led_power_get_uri = {
            .uri = "/api/v1/led/power",
            .method = HTTP_GET,
            .handler = led_power_get_handler,
            .user_ctx = this
    };
    httpd_register_uri_handler(m_hdnlServer, &led_power_get_uri);
    httpd_uri_t led_power_post_uri = {
            .uri = "/api/v1/led/power",
            .method = HTTP_POST,
            .handler = led_power_post_handler,
            .user_ctx = this
    };
    httpd_register_uri_handler(m_hdnlServer, &led_power_post_uri);

    httpd_uri_t led_mode_get_uri = {
            .uri = "/api/v1/led/mode",
            .method = HTTP_GET,
            .handler = led_mode_get_handler,
            .user_ctx = this
    };
    httpd_register_uri_handler(m_hdnlServer, &led_mode_get_uri);
    httpd_uri_t led_mode_post_uri = {
            .uri = "/api/v1/led/mode",
            .method = HTTP_POST,
            .handler = led_mode_post_handler,
            .user_ctx = this
    };
    httpd_register_uri_handler(m_hdnlServer, &led_mode_post_uri);
    httpd_uri_t led_modes_get_uri = {
            .uri = "/api/v1/led/modes",
            .method = HTTP_GET,
            .handler = led_modes_get_handler,
            .user_ctx = this
    };
    httpd_register_uri_handler(m_hdnlServer, &led_modes_get_uri);

    httpd_uri_t config_get_uri = {
            .uri = "/api/v1/config",
            .method = HTTP_GET,
            .handler = config_get_handler,
            .user_ctx = this
    };
    httpd_register_uri_handler(m_hdnlServer, &config_get_uri);
    httpd_uri_t config_post_uri = {
            .uri = "/api/v1/config",
            .method = HTTP_POST,
            .handler = config_post_handler,
            .user_ctx = this
    };
    httpd_register_uri_handler(m_hdnlServer, &config_post_uri);

    httpd_uri_t file_get_uri = {
            .uri = "/*",
            .method = HTTP_GET,
            .handler = file_get_handler
    };
    httpd_register_uri_handler(m_hdnlServer, &file_get_uri);
}

esp_err_t WebServer::jsonResponse(cJSON *root, httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");

    const char *responsePayload = cJSON_Print(root);
    httpd_resp_sendstr(req, responsePayload);

    free((void *)responsePayload);
    cJSON_Delete(root);

    return ESP_OK;
}

esp_err_t WebServer::handlePost(httpd_req_t *req, const std::function<bool(cJSON *root)>& jsonHandler)
{
    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = m_readBuffer;
    int received = 0;
    if (total_len >= READ_BUFFER_SIZE) {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len) {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0) {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';

    cJSON *root = cJSON_Parse(buf);
    if (!root) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Failed to parse JSON");
    } else {
        if (!jsonHandler(root)) {
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "JSON is missing keys or has invalid values");
        }
    }

    cJSON_Delete(root);
    httpd_resp_sendstr(req, "Post control value successfully");
    return ESP_OK;
}
