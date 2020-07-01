#include "ModeOptionsPersister.h"
#include "LedMode/LedMode.h"
#include <cJSON.h>
#include <esp_log.h>
#include <cstdio>
#include <cstring>
#include <sys/unistd.h>

namespace LedWall {

static const char *LOG_TAG = "ModeOptionsPersister";
static const int READ_BUFFER_SIZE = 2 * 1024;
static const int MAX_FILE_NAME_LENGTH = 10;

bool ModeOptionsPersister::saveOptions(Mode::LedMode *ledMode, const char *modeName)
{
    ESP_LOGI(LOG_TAG, "saveOptions %s", modeName);

    if (strlen(modeName) > MAX_FILE_NAME_LENGTH) {
        ESP_LOGE(LOG_TAG, "saveOptions, mode name is too long, options can not be saved");
        return false;
    }

    char *cfgFileName = (char*)calloc(MAX_FILE_NAME_LENGTH, sizeof(char));
    sprintf(cfgFileName, "/%s", modeName);

    cJSON *root = cJSON_CreateObject();
    ledMode->readOptions(root);

    const char *writeBuffer = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    if (strlen(writeBuffer) > READ_BUFFER_SIZE) {
        ESP_LOGE(LOG_TAG, "saveOptions, config data is too big and can not be saved");
        free((void *)cfgFileName);
        free((void *)writeBuffer);
        return false;
    }

    FILE *cfgFile = fopen(cfgFileName, "w");
    if (cfgFile == nullptr) {
        ESP_LOGE(LOG_TAG, "saveOptions, Failed to open file : %s", cfgFileName);
        free((void *)cfgFileName);
        free((void *)writeBuffer);
        return false;
    }

    if (1 != fwrite(writeBuffer, strlen(writeBuffer), 1, cfgFile)) {
        ESP_LOGE(LOG_TAG, "saveOptions, Failed to write file : %s", cfgFileName);
        fclose(cfgFile);
        free((void *)cfgFileName);
        free((void *)writeBuffer);
        return false;
    }

    fclose(cfgFile);
    free((void *)cfgFileName);
    free((void *)writeBuffer);

    return true;
}

bool ModeOptionsPersister::loadOptions(Mode::LedMode *ledMode, const char *modeName)
{
    ESP_LOGI(LOG_TAG, "loadOptions %s", modeName);

    if (strlen(modeName) > MAX_FILE_NAME_LENGTH) {
        ESP_LOGE(LOG_TAG, "loadOptions, mode name is too long, options can not be saved");
        return false;
    }

    char *cfgFileName = (char*)calloc(MAX_FILE_NAME_LENGTH, sizeof(char));
    sprintf(cfgFileName, "/%s", modeName);

    FILE *cfgFile = fopen(cfgFileName, "r");
    if (cfgFile == nullptr) {
        ESP_LOGE(LOG_TAG, "loadOptions, Failed to open file : %s", cfgFileName);
        free((void *)cfgFileName);
        return false;
    }

    char *readBuffer = (char*)calloc(READ_BUFFER_SIZE, sizeof(char));
//    fread(readBuffer, READ_BUFFER_SIZE, 1, cfgFile);
    fread(readBuffer, sizeof(char), READ_BUFFER_SIZE, cfgFile);
    if (ferror(cfgFile)) {
        ESP_LOGE(LOG_TAG, "loadOptions, Failed to read file : %s", cfgFileName);
        fclose(cfgFile);
        free((void *)cfgFileName);
        free(readBuffer);
        return false;
    }
    fclose(cfgFile);
    free((void *)cfgFileName);

    cJSON *modeConfig = cJSON_Parse(readBuffer);
    ledMode->writeOptions(modeConfig);

    cJSON_free(modeConfig);
    free(readBuffer);

    return true;
}

void ModeOptionsPersister::resetOptions(Mode::LedMode *ledMode, const char *modeName)
{
    ESP_LOGI(LOG_TAG, "resetOptions %s", modeName);

    unlink(modeName);
}

} // namespace LedWall
