#include "PresetManager.h"
#include <esp_log.h>
#include <cJSON.h>
#include <cstring>
#include <algorithm>
#include "../LedMode/LedMode.h"

/*
 * '/presets.json'
 * {
 *  "presets": [
 *      {
 *          name: 'my cool preset',
 *          mode: 'Wave',
 *          options: { ... }
 *      },
 *      {
 *          name: 'awesome preset',
 *          mode: 'MultiBars',
 *          ...
 *      },
 *      {
 *          ...
 *      }
 *  ]
 * }
 */

namespace LedWall {

static const char *LOG_TAG = "PresetManager";
static const char *presetsFileName = "/presets.json";
static const int FILE_BUFFER_SIZE = 2 * 1024; // used for read and write

PresetManager::~PresetManager()
{
    closePresetsFile();
}

PresetList PresetManager::getPresets()
{
    ESP_LOGD(LOG_TAG, "getPresets");

    if (readPresetsFromFile()) {
        ESP_LOGD(LOG_TAG, "getPresets, readPresets succeed: %d presets available", m_presetList.size());
    }

    return m_presetList;
}

bool PresetManager::hasPreset(const std::string &presetName)
{
    ESP_LOGD(LOG_TAG, "hasPreset(%s)", presetName.c_str());

    if (!Preset::isValidName(presetName)) {
        return false;
    }

    getPresets(); // FIXME [load] make sure load-from-file is triggered

    return findPresetByName(presetName) != m_presetList.end();
}

Preset PresetManager::getPreset(const std::string &presetName)
{
    ESP_LOGD(LOG_TAG, "getPreset(%s)", presetName.c_str());

    // FIXME [load] relies on hasPreset to load-from-file
    if (!hasPreset(presetName)) {
        return Preset();
    }

    return *findPresetByName(presetName);
}

bool PresetManager::savePreset(const std::string &presetName, const char *modeName, Mode::LedMode *ledMode)
{
    ESP_LOGD(LOG_TAG, "savePreset(%s)", presetName.c_str());

    if (!Preset::isValidName(presetName)) {
        return false;
    }

    // FIXME [load] relies on hasPreset to load-from-file
    if (hasPreset(presetName)) {
        deletePreset(presetName);
    }

    createPreset(presetName, modeName, ledMode);
    return writePresetsToFile();
}

bool PresetManager::deletePreset(const std::string &presetName)
{
    ESP_LOGD(LOG_TAG, "deletePreset(%s)", presetName.c_str());

    // FIXME [load] relies on hasPreset to load-from-file
    if (!hasPreset(presetName)) {
        return false;
    }

    m_presetList.erase(findPresetByName(presetName));
    return writePresetsToFile();
}

bool PresetManager::deleteAllPresets()
{
    ESP_LOGD(LOG_TAG, "deleteAllPresets");

    m_presetList.clear();
    return writePresetsToFile();
}

bool PresetManager::openPresetsFile(bool openForWriting)
{
    ESP_LOGV(LOG_TAG, "openPresetsFile(%s)", (openForWriting ? "w" : "r"));

    if (m_presetsFile != nullptr) {
        // don't know if its opened for read or write -> error
        ESP_LOGE(LOG_TAG, "openPresetsFile, already open");
        return false;
    }

    m_presetsFile = std::fopen(presetsFileName, (openForWriting ? "w" : "r"));
    if (m_presetsFile == nullptr) {
        ESP_LOGE(LOG_TAG, "openPresetsFile, Failed to open file: '%s' (%s): %s", presetsFileName, (openForWriting ? "w" : "r"), std::strerror(errno));
        return false;
    }

    return true;
}

void PresetManager::closePresetsFile()
{
    ESP_LOGV(LOG_TAG, "closePresetsFile");

    if (m_presetsFile == nullptr) {
        return;
    }

    std::fclose(m_presetsFile);
    m_presetsFile = nullptr;
}

bool PresetManager::readPresetsFromFile()
{
    ESP_LOGV(LOG_TAG, "readPresetsFromFile");

    m_presetList.clear();

    if (!openPresetsFile()) {
        return false;
    }

    char *readBuffer = (char*)calloc(FILE_BUFFER_SIZE, sizeof(char));
    std::fread(readBuffer, sizeof(char), FILE_BUFFER_SIZE, m_presetsFile);
    if (std::ferror(m_presetsFile)) {
        ESP_LOGE(LOG_TAG, "readPresetsFromFile, Failed to read file: '%s'", presetsFileName);
        free(readBuffer);
        closePresetsFile();
        return false;
    }

    cJSON *presetsJson = cJSON_Parse(readBuffer);
    free(readBuffer);
    closePresetsFile();

    cJSON *allPresets = cJSON_GetObjectItem(presetsJson, "presets");
    if (!cJSON_IsArray(allPresets)) {
        ESP_LOGW(LOG_TAG, "readPresetsFromFile, 'presets' is not an array, no presets to load");
        cJSON_free(allPresets);
        return false;
    }

    if (cJSON_IsObject(allPresets->child)) {
        auto *currentPreset = allPresets->child;
        do {
            m_presetList.push_back(readPreset(currentPreset));
            currentPreset = currentPreset->next;
        } while (nullptr != currentPreset);
    } else {
        ESP_LOGW(LOG_TAG, "readPresetsFromFile, first child of 'presets' is not an object, failed to load presets");
    }

    ESP_LOGD(LOG_TAG, "readPresetsFromFile, read %d presets from disc", m_presetList.size());

    cJSON_free(allPresets);
    return true;
}

bool PresetManager::writePresetsToFile()
{
    ESP_LOGV(LOG_TAG, "writePresetsToFile");

    cJSON *root = cJSON_CreateObject();
    cJSON *allPresets = cJSON_AddArrayToObject(root, "presets");
    for (auto &preset : m_presetList) {
        cJSON *presetJson = writePreset(preset);
        cJSON_AddItemToArray(allPresets, presetJson);
    }

    char *writeBuffer = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    if (strlen(writeBuffer) > FILE_BUFFER_SIZE) {
        ESP_LOGE(LOG_TAG, "writePresetsToFile, presets data is too big and can not be saved");
        free(writeBuffer);
        return false;
    }

    if (!openPresetsFile(true)) {
        free(writeBuffer);
        return false;
    }

    if (1 != fwrite(writeBuffer, strlen(writeBuffer), 1, m_presetsFile)) {
        ESP_LOGE(LOG_TAG, "writePresetsToFile, Failed to write file: '%s'", presetsFileName);
        free(writeBuffer);
        closePresetsFile();
        return false;
    }

    ESP_LOGD(LOG_TAG, "writePresetsToFile, written %d presets to disc", m_presetList.size());

    free(writeBuffer);
    closePresetsFile();

    return true;
}

Preset PresetManager::readPreset(cJSON *presetJson)
{
    ESP_LOGV(LOG_TAG, "readPreset");

    Preset preset;

    auto presetName = cJSON_GetObjectItem(presetJson, "name");
    if (!cJSON_IsString(presetName)) {
        ESP_LOGE(LOG_TAG, "readPreset, Failed to read preset: no name defined");
        return preset;
    }
    preset.setPresetName(presetName->valuestring);

    auto presetMode = cJSON_GetObjectItem(presetJson, "mode");
    if (!cJSON_IsString(presetMode)) {
        ESP_LOGE(LOG_TAG, "readPreset, Failed to read preset: no mode defined");
        return Preset();
    }
    preset.setModeName(presetMode->valuestring);

    auto presetOptions = cJSON_GetObjectItem(presetJson, "options");
    if (presetOptions == cJSON_DetachItemViaPointer(presetJson, presetOptions)) {
        preset.setModeOptions(presetOptions);
    }

    return preset;
}

cJSON *PresetManager::writePreset(const Preset &preset)
{
    ESP_LOGV(LOG_TAG, "writePreset");

    return writePreset(
            preset.getPresetName().c_str(),
            preset.getModeName().c_str(),
            preset.getModeOptions()
            );
}

cJSON *PresetManager::writePreset(const char *presetName, const char *modeName, cJSON *modeOptions)
{
    ESP_LOGV(LOG_TAG, "writePreset");

    assert(presetName != nullptr);
    assert(modeName != nullptr);
    assert(modeOptions != nullptr);

    cJSON *presetJson = cJSON_CreateObject();
    cJSON_AddStringToObject(presetJson, "name", presetName);
    cJSON_AddStringToObject(presetJson, "mode", modeName);

    auto copyOfModeOptions = cJSON_Duplicate(modeOptions, true);
    cJSON_AddItemToObject(presetJson, "options", copyOfModeOptions);

    return presetJson;
}

PresetList::iterator PresetManager::findPresetByName(const std::string &presetName)
{
    return std::find_if(m_presetList.begin(), m_presetList.end(), [presetName](const Preset &preset) {
        return preset.getPresetName() == presetName;
    });
}

void PresetManager::createPreset(const std::string &presetName, const char *modeName, Mode::LedMode *ledMode)
{
    ESP_LOGV(LOG_TAG, "createPreset(%s)", presetName.c_str());

    cJSON *modeOptions = cJSON_CreateObject();
    ledMode->readOptions(modeOptions);

    Preset preset;
    preset.setPresetName(presetName);
    preset.setModeName(modeName);
    preset.setModeOptions(modeOptions);
    m_presetList.push_back(preset);
}

}
