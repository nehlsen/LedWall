#include "FancyScript.h"
#include <utility>
#include "FancyParts/FancyPartFactory.h"

/*
 *
 *  line: MODE,OP,ARGs
 *  MODE : string
 *  OP : integer, internal options like forward/reverse, horiz/vert
 *  ARGs : arguments to the part itself
 *
 */

namespace LedWall::Mode {

FancyScript::FancyScript(LedMatrix &matrix) : LedMode(matrix)
{
    setScript({
        "ExpLet,0,5",
        "ExpLet,0,4",
        "ExpLet,0,3",
        "ExpLet,0,2",
        "ExpLet,0,1",
        "ExpLet,0,2022",
        "Sprinkle",
    });
}

void FancyScript::setScript(std::vector<std::string> lines)
{
    // TODO protect other methods from "no lines at all" e.g. always have "nothing mode"
    m_lines = std::move(lines);
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
