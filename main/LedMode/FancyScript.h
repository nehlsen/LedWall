#ifndef LEDWALL_FANCYSCRIPT_H
#define LEDWALL_FANCYSCRIPT_H

#include "LedMode.h"
#include "FancyParts/FancyPart.h"
#include <vector>
#include <string>

namespace LedWall::Mode {

class FancyScript : public LedMode
{
public:
    explicit FancyScript(LedMatrix& matrix);

    void setScript(std::vector<std::string> lines);

    void setScript(const std::string& script);
    std::string getScript() const;

    bool update() override;

    void readOptions(cJSON *root) override;
    bool writeOptions(cJSON *root) override;

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
