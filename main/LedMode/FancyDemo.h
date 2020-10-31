#ifndef LEDWALL_FANCYDEMO_H
#define LEDWALL_FANCYDEMO_H

#include "LedMode.h"

namespace LedWall::Mode {

class FancyDemoPart;
class FancyDemo : public LedMode
{
public:
    explicit FancyDemo(LedMatrix& matrix);

    bool update() override;

protected:
    uint16_t m_frame = 0;

    int16_t part6a(int16_t frame);
    int16_t part6b(int16_t frame);
    int16_t part6_abs(int16_t frame, bool forward);

    std::vector<FancyDemoPart*> m_parts;
    void initParts();

    FancyDemoPart* getCurrentPart();
};

}

#endif //LEDWALL_FANCYDEMO_H
