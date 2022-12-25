#ifndef LEDWALL_FANCYSCRIPT_H
#define LEDWALL_FANCYSCRIPT_H

#include "LedMode.h"
#include "LedMode/FancyParts/FancyPart.h"
#include <vector>

namespace LedWall::Mode {

class FancyScript : public LedMode
{
public:
    explicit FancyScript(LedMatrix& matrix);

    void setScript(std::vector<std::string> lines);

    bool update() override;

protected:
    std::vector<std::string> m_lines;
    uint16_t m_currentLine = 0;

    FancyParts::FancyPart* m_currentPart = nullptr;
    FancyParts::FancyPart* getCurrentPart();

    uint16_t m_partFrame = 0;

    FancyParts::FancyPart* createPart(const std::string& line);
    static std::string extractPartName(const std::string& line);
    static FancyParts::PartOptions extractOptions(const std::string& line);
    static std::string extractArguments(const std::string& line);
};

}

#endif //LEDWALL_FANCYSCRIPT_H
