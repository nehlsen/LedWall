#include "ModeOptionsPersister.h"
#include "LedMode/LedMode.h"
#include <cJSON.h>
#include <esp_log.h>
#include <cstring>
#include <sys/unistd.h>

namespace LedWall {

static const char *LOG_TAG = "ModeOptionsPersister";
static const int READ_BUFFER_SIZE = 2 * 1024;
static const int MAX_FILE_NAME_LENGTH = 10;

bool ModeOptionsPersister::save(Mode::LedMode *ledMode, const char *modeName)
{
    return ModeOptionsPersister().saveOptions(ledMode, modeName);
}

bool ModeOptionsPersister::load(Mode::LedMode *ledMode, const char *modeName)
{
    return ModeOptionsPersister().loadOptions(ledMode, modeName);
}

bool ModeOptionsPersister::reset(Mode::LedMode *ledMode, const char *modeName)
{
    return ModeOptionsPersister().resetOptions(ledMode, modeName);
}

ModeOptionsPersister::ModeOptionsPersister()
{
    m_cfgFileName = (char*)calloc(MAX_FILE_NAME_LENGTH, sizeof(char));
    m_readBuffer = (char*)calloc(READ_BUFFER_SIZE, sizeof(char));
}

ModeOptionsPersister::~ModeOptionsPersister()
{
    free(m_cfgFileName);
    free(m_readBuffer);
    free(m_writeBuffer);
    fclose(m_cfgFile);
}

bool ModeOptionsPersister::saveOptions(Mode::LedMode *ledMode, const char *modeName)
{
    ESP_LOGD(LOG_TAG, "saveOptions %s", modeName);

    if (strlen(modeName) > MAX_FILE_NAME_LENGTH) {
        ESP_LOGE(LOG_TAG, "saveOptions, mode name is too long, options can not be saved");
        return false;
    }

    sprintf(m_cfgFileName, "/%s", modeName);

    cJSON *root = cJSON_CreateObject();
    ledMode->readOptions(root);

    m_writeBuffer = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    if (strlen(m_writeBuffer) > READ_BUFFER_SIZE) {
        ESP_LOGE(LOG_TAG, "saveOptions, config data is too big and can not be saved");
        return false;
    }

    m_cfgFile = fopen(m_cfgFileName, "w");
    if (m_cfgFile == nullptr) {
        ESP_LOGE(LOG_TAG, "saveOptions, Failed to open file : %s", m_cfgFileName);
        return false;
    }

    if (1 != fwrite(m_writeBuffer, strlen(m_writeBuffer), 1, m_cfgFile)) {
        ESP_LOGE(LOG_TAG, "saveOptions, Failed to write file : %s", m_cfgFileName);
        return false;
    }

    return true;
}

bool ModeOptionsPersister::loadOptions(Mode::LedMode *ledMode, const char *modeName)
{
    ESP_LOGD(LOG_TAG, "loadOptions %s", modeName);

    if (strlen(modeName) > MAX_FILE_NAME_LENGTH) {
        ESP_LOGE(LOG_TAG, "loadOptions, mode name is too long, options can not be saved");
        return false;
    }

    sprintf(m_cfgFileName, "/%s", modeName);

    m_cfgFile = fopen(m_cfgFileName, "r");
    if (m_cfgFile == nullptr) {
        ESP_LOGE(LOG_TAG, "loadOptions, Failed to open file : %s", m_cfgFileName);
        return false;
    }

//    fread(readBuffer, READ_BUFFER_SIZE, 1, cfgFile);
    fread(m_readBuffer, sizeof(char), READ_BUFFER_SIZE, m_cfgFile);
    if (ferror(m_cfgFile)) {
        ESP_LOGE(LOG_TAG, "loadOptions, Failed to read file : %s", m_cfgFileName);
        return false;
    }

    cJSON *modeConfig = cJSON_Parse(m_readBuffer);
    ledMode->writeOptions(modeConfig);

    cJSON_free(modeConfig);

    return true;
}

bool ModeOptionsPersister::resetOptions(Mode::LedMode *ledMode, const char *modeName)
{
    ESP_LOGD(LOG_TAG, "resetOptions %s", modeName);

    unlink(modeName);

    return true;
}

} // namespace LedWall
