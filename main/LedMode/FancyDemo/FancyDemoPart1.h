#ifndef LEDWALL_FANCYDEMOPART1_H
#define LEDWALL_FANCYDEMOPART1_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

// start with centered box, grow bars horizontally
class FancyDemoPart1 : public FancyDemoPart
{
public:
    using FancyDemoPart::FancyDemoPart;
//    explicit FancyDemoPart1(uint16_t firstFrame, LedMatrix &matrix);

    void render(uint16_t relativeFrame) override;

protected:
    uint16_t getFrameCount() const override;
};

}

#endif //LEDWALL_FANCYDEMOPART1_H
