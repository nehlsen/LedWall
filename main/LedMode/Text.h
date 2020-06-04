#ifndef LEDWALL_TEXT_H
#define LEDWALL_TEXT_H

#include "LedMode.h"
#include <string>

namespace LedWall {
namespace Mode {

class Text : public LedMode
{
public:
    using LedMode::LedMode;

    bool update() override;

    void readOptions(cJSON *root) override;
    bool writeOptions(cJSON *root) override;

    const std::string &getText() const;
    void setText(const std::string &text);

    uint8_t getScrollSpeed() const;
    void setScrollSpeed(uint8_t scrollSpeed);

protected:
    std::string m_text = "<--=";
    CRGB m_fgColor;
    CRGB m_bgColor;

    uint8_t m_scrollSpeed = 40; // pixels per second
    int64_t m_lastUpdate = 0;

//    Direction m_scrollDirection; // horizontal / vertical
    // scroll repeat - bounce / flow
    //  bounce back at edges
    //  flow continuous

    int m_currentStep = 0;
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_TEXT_H
