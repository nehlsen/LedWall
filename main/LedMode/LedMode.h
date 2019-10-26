#ifndef LEDWALL_LEDMODE_H
#define LEDWALL_LEDMODE_H

class CRGB;

class LedMode
{
public:
    LedMode(CRGB *leds, int ledCount);
    virtual ~LedMode();

    virtual void update() = 0;

protected:
    CRGB *m_leds;
    int m_ledCount;
};

#endif //LEDWALL_LEDMODE_H
