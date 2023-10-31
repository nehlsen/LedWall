#include "FancyScript.h"
#include <utility>
#include <sstream>
#include "FancyParts/FancyPartFactory.h"
#include <cJSON.h>
#include <esp_log.h>

/*
 *
 *  line: MODE,OP,ARGs
 *  MODE : string
 *  OP : integer, internal options like forward/reverse, horiz/vert
 *  ARGs : arguments to the part itself
 *
 */

#define JSON_MAX_LINE_LENGTH 32
#define JSON_MAX_LINE_COUNT 100

namespace LedWall::Mode {

FancyScript::FancyScript(LedMatrix &matrix) : LedMode(matrix)
{
    setScript({
//        "ExpLet,0,5",
//        "ExpLet,0,4",
//        "ExpLet,0,3",
//        "ExpLet,0,2",
//        "ExpLet,0,1",
//        "ExpLet,0,2022",
//        "Sprinkle",
//        "ExpLet,1,2022", // TODO hold the result for 1-2 seconds
//        "ColorRays,1",
//        "ColorRays",
//        "ExpLet,0,2022",
        "BarsGrow",
        "BarsOpen",
//        "ExpLet,1,2022",
        "BarsOpen,1",
        "BarsGrow,1",
//        "ExpLet,0,2022",
        "CubeGrow,1",
        "CubeRoll",
//        "ColorRays",
        "CircleGrow",
//        "ColorRays,1",
        "CircleGrow,1",
        "CubeRoll,1",
        "BarsGrow,2",
        "BarsOpen,2",
        "CircleGrow,1",
        "CircleGrow,1",
//        "ExpLet,1,2022",
        "BarsOpen,3",
        "BarsGrow,3",
        "CN",
    });
}

void FancyScript::setScript(std::vector<std::string> lines)
{
    // TODO protect other methods from "no lines at all" e.g. always have "nothing mode"
    m_lines = std::move(lines);
}

void FancyScript::setScript(const std::string& script)
{
    m_lines.clear();

    std::istringstream i(script);
    std::string buf;
    while(getline(i, buf, '\n')) {
        m_lines.push_back(buf);
    }
}

std::string FancyScript::getScript() const
{
    std::string lines;
    std::ostringstream o(lines);

    for (const auto & line : m_lines) {
        o << line << "\n";
    }

    return lines;
}

bool FancyScript::update()
{
    auto part = getCurrentPart();
    part->render(m_partFrame);
    ++m_partFrame;

    if (m_partFrame >= part->getFrameCount()) {
        delete m_currentPart;
        m_currentPart = nullptr;
        ++m_currentLine;
        m_partFrame = 0;
    }

    return true;
}

void FancyScript::readOptions(cJSON *root)
{
    cJSON *const script = cJSON_AddArrayToObject(root, "script");

    for (const auto& line : m_lines) {
        cJSON *const jsonLine = cJSON_CreateString(line.c_str());
        if (!jsonLine || !cJSON_AddItemToArray(script, jsonLine)) {
            ESP_LOGE("FancyScript", "Failed to write JSON - maybe out of memory?");
            cJSON_Delete(script);
            return;
        }
    }
}

bool FancyScript::writeOptions(cJSON *root)
{
    cJSON *const script = cJSON_GetObjectItem(root, "script");
    if (!cJSON_IsArray(script)) {
        return false;
    }

    std::vector<std::string> newLines;

    cJSON* item;
    cJSON_ArrayForEach(item, script) {
        if (!cJSON_IsString(item)) {
            continue;
        }
        if (strlen(item->valuestring) > JSON_MAX_LINE_LENGTH) {
            continue;
        }

        newLines.emplace_back(item->valuestring);

        if (newLines.size() > JSON_MAX_LINE_COUNT) {
            break;
        }
    }

    m_lines = std::move(newLines);
    return true;
}

FancyParts::FancyPart *FancyScript::getCurrentPart()
{
    if (nullptr != m_currentPart) {
        return m_currentPart;
    }

    if (m_currentLine >= m_lines.size()) {
        m_currentLine = 0;
    }

    m_currentPart = createPart(m_lines.at(m_currentLine));
    return m_currentPart;
}

FancyParts::FancyPart *FancyScript::createPart(const std::string& line)
{
    const auto partName = extractPartName(line);
    const auto options = extractOptions(line);
    const auto arguments = extractArguments(line);

    return FancyParts::FancyPartFactory::create(partName, m_matrix, options, arguments);
}

std::string FancyScript::extractPartName(const std::string &line)
{
    const auto firstDelimiter = line.find(',');
    if (firstDelimiter == std::string::npos) {
        return line;
    }

    return line.substr(0, firstDelimiter);
}

FancyParts::PartOptions FancyScript::extractOptions(const std::string &line)
{
    const auto firstDelimiter = line.find(',');
    if (firstDelimiter == std::string::npos) {
        return FancyParts::NO_OPS;
    }

    std::string sops;
    const auto secondDelimiter = line.find(',', firstDelimiter + 1);
    if (secondDelimiter == std::string::npos) {
        sops = line.substr(firstDelimiter + 1);
    } else {
        sops = line.substr(firstDelimiter + 1, secondDelimiter - (firstDelimiter + 1));
    }

    return static_cast<FancyParts::PartOptions>(std::stoi(sops.empty() ? "0" : sops));
}

std::string FancyScript::extractArguments(const std::string &line)
{
    const auto firstDelimiter = line.find(',');
    const auto secondDelimiter = line.find(',', firstDelimiter + 1);
    if (firstDelimiter == std::string::npos || secondDelimiter == std::string::npos) {
        return "";
    }

    return line.substr(secondDelimiter + 1);
}

}
