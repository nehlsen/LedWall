#include "OtaUpdater.h"
#include <esp_https_ota.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_log.h>
#include <esp_ota_ops.h>

static const char *UPDATER_LOG_TAG = "OtaUpdater";

#define UPDATER_RUNNING_BIT BIT1
static EventGroupHandle_t ota_updater_event_group;

class OtaUpdaterPrivate
{
public:
    OtaUpdaterPrivate():
        httpConfig(), otaConfig({.http_config = &httpConfig})
    {}

    bool isActive() const { return nullptr != otaHandle; }

    esp_http_client_config_t httpConfig;
    esp_https_ota_config_t otaConfig;
    esp_https_ota_handle_t otaHandle = nullptr;

    esp_err_t lastError = ESP_OK;

    void work()
    {
        esp_app_desc_t updateInfo;
        lastError = esp_https_ota_get_img_desc(otaHandle, &updateInfo);
        if (updateInfo.magic_word != ESP_APP_DESC_MAGIC_WORD) {
            ESP_LOGE(UPDATER_LOG_TAG, "Update info: magic word mismatch!");
            lastError = ESP_ERR_OTA_VALIDATE_FAILED;
        }
        if (lastError == ESP_OK) {
            ESP_LOGI(
                    UPDATER_LOG_TAG,
                    "Update info: name:%s, version:%s, compiled: %s / %s",
                    updateInfo.project_name,
                    updateInfo.version,
                    updateInfo.date,
                    updateInfo.time
            );

            while (true) { // download loop
                lastError = esp_https_ota_perform(otaHandle);
                if (lastError != ESP_ERR_HTTPS_OTA_IN_PROGRESS) {
                    // something failed. e.g. download failed, failed to write to flash, ...
                    // OR: ESP_OK: download complete
                    break;
                }
            }
        }

        // cleanup broken update OR finish successful update
        esp_err_t ota_finish_err = esp_https_ota_finish(otaHandle); // free's otaHandle
        otaHandle = nullptr;

        if (lastError == ESP_OK) {
            // if esp_https_ota_perform() was successful, overwrite last-error with the one from finish
            lastError = ota_finish_err;
        }
    }
};

/**********************************************************************************************************************/

void ota_updater_task(void *pvParameter)
{
    ESP_LOGI(UPDATER_LOG_TAG, "Starting OTA Update Task...");

    auto updater = (OtaUpdaterPrivate*)pvParameter;

    while (true) { // this will only loop if the update fails
        xEventGroupWaitBits(ota_updater_event_group, UPDATER_RUNNING_BIT, false, false, portMAX_DELAY);
        ESP_LOGI(UPDATER_LOG_TAG, "Starting Update...");
        updater->work();
        xEventGroupClearBits(ota_updater_event_group, UPDATER_RUNNING_BIT);
    }
}

/**********************************************************************************************************************/

OtaUpdater::OtaUpdater():
    m_updater(new OtaUpdaterPrivate)
{
    esp_log_level_set("esp_https_ota", ESP_LOG_DEBUG); // log download progress

    ota_updater_event_group = xEventGroupCreate();
    xTaskCreate(
            &ota_updater_task,
            "ota_updater_task",
            4000,
            m_updater,
            1,
            nullptr
    );
}

void OtaUpdater::setUpdateUrl(const char *url)
{
    if (m_updater->isActive()) {
        return;
    }

    m_updater->httpConfig.url = url;
}

bool OtaUpdater::startUpdate()
{
    if (m_updater->isActive()) {
        return false;
    }

    m_updater->lastError = esp_https_ota_begin(&m_updater->otaConfig, &m_updater->otaHandle);
    if (m_updater->lastError == ESP_OK) {
        xEventGroupSetBits(ota_updater_event_group, UPDATER_RUNNING_BIT);
        return true;
    }

    return false;
}

esp_err_t OtaUpdater::getLastError() const
{
    return m_updater->lastError;
}
