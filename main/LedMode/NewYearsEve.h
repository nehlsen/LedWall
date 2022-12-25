#ifndef LEDWALL_NEWYEARSEVE_H
#define LEDWALL_NEWYEARSEVE_H

#include "ModeText.h"
#include "FancyScript.h"
#include <vector>
#include <ctime>

namespace LedWall::Mode {

class NewYearsEve : public LedMode
{
public:
    explicit NewYearsEve(LedMatrix& matrix);

    bool update() override;

protected:
    ModeText* m_countdownText = nullptr;
    FancyScript* m_fancyAnimation = nullptr;

//    bool playParts();
    bool renderCountdown(int remainingSeconds);

//    uint16_t m_frame = 0;

//    std::vector<FancyDemoPart*> m_parts;
//    void initParts();
//
//    FancyDemoPart* getCurrentPart();

    std::tm m_targetTime;
    void initTargetTime();
    int secondsUntilNewYears();

    void initCurtain();
    void initFancyAnimation();
};

}

#endif //LEDWALL_NEWYEARSEVE_H
