#ifndef LEDWALL_TEXT_H
#define LEDWALL_TEXT_H

#include "LedMode.h"
#include <string>
#include <Text.h>

namespace LedWall {
namespace Mode {

class ModeText : public LedMode
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

    enum ScrollDirection {
        ScrollLeft = 0,
        ScrollRight
    };
    ScrollDirection getScrollDirection() const;
    void setScrollDirection(ScrollDirection direction);

    enum ScrollMode {
        ScrollNone = 0,
        ScrollInfinite,
        ScrollBounce
    };
    ScrollMode getScrollMode() const;
    void setScrollMode(ScrollMode mode);

protected:
    ::Text m_displayText;
    CRGB m_fgColor;
    CRGB m_bgColor;

    uint8_t m_scrollSpeed = 40; // pixels per second
    int64_t m_lastUpdate = 0;

    ScrollDirection m_scrollDirection = ScrollLeft;
    ScrollMode m_scrollMode = ScrollInfinite;

    int m_currentStep = 0;

    int scrollInfinite();
    int scrollBounce();
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_TEXT_H
