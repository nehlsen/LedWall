#ifndef LEDWALL_FANCYDEMO_H
#define LEDWALL_FANCYDEMO_H

#include "LedMode.h"
#include <vector>

namespace LedWall::Mode {

class FancyDemoPart;
class FancyDemo : public LedMode
{
public:
    explicit FancyDemo(LedMatrix& matrix);

    bool update() override;

protected:
    uint16_t m_frame = 0;

    std::vector<FancyDemoPart*> m_parts;
    void initParts();

    FancyDemoPart* getCurrentPart();
};

}

#endif //LEDWALL_FANCYDEMO_H
