#ifndef LEDWALL_NEWYEARSEVE_H
#define LEDWALL_NEWYEARSEVE_H

#include "ModeText.h"
#include <vector>
#include <ctime>

namespace LedWall::Mode {

class FancyDemoPart;
class NewYearsEve : public ModeText
{
public:
    explicit NewYearsEve(LedMatrix& matrix);

    bool update() override;

protected:
    bool playParts();
    bool renderCountdown(int remainingSeconds);

    uint16_t m_frame = 0;

    std::vector<FancyDemoPart*> m_parts;
    void initParts();

    FancyDemoPart* getCurrentPart();

    std::tm m_targetTime;
    void initTargetTime();
    int secondsUntilNewYears();
};

}

#endif //LEDWALL_NEWYEARSEVE_H
