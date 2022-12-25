#include "NewYearsEve.h"
#include "TimeFormatter.h"
#include <algorithm>
#include <esp_log.h>
#include "FancyDemo.h"
#include "ModeText.h"

//#include <iostream>

namespace LedWall::Mode {

static const uint8_t curtainTime = 9;

NewYearsEve::NewYearsEve(LedMatrix &matrix):
    LedMode(matrix), m_fancyAnimation(new FancyScript(matrix))
{
    initTargetTime();
    initCurtain();
}

bool NewYearsEve::update()
{
    int remainingSeconds = secondsUntilNewYears();
    if (remainingSeconds > curtainTime) {
        return renderCountdown(remainingSeconds);
    } else if (remainingSeconds > 1 ) {
        return m_fancyAnimation->update();
    } else if (remainingSeconds > 0) {
        initFancyAnimation();
    }

    return m_fancyAnimation->update();
}

bool NewYearsEve::renderCountdown(int remainingSeconds)
{
    if (m_countdownText == nullptr) {
        m_countdownText = new ModeText(m_matrix);
        m_countdownText->setScrollMode(ModeText::ScrollNone);
    }

    m_countdownText->setText(TimeFormatter::formatSeconds(remainingSeconds, remainingSeconds%2));
    return m_countdownText->update();
}

void NewYearsEve::initTargetTime()
{
    std::time_t tnow = std::time(nullptr);
    std::tm *tmnow = std::localtime(&tnow);
    memcpy(&m_targetTime, tmnow, sizeof(std::tm));

    if (false) {
//        m_targetTime.tm_mday += 1;
//        m_targetTime.tm_hour += 1;
//        m_targetTime.tm_min += 2;
        m_targetTime.tm_sec += 18;
//        m_targetTime.tm_sec += 1;
    } else {
        m_targetTime.tm_year += 1;
        m_targetTime.tm_mon = 0;
        m_targetTime.tm_mday = 1;
        m_targetTime.tm_hour = 0;
        m_targetTime.tm_min = 0;
        m_targetTime.tm_sec = 0;
    }

    char s_targetTime[100];
    strftime(s_targetTime, sizeof s_targetTime, "%c\n", &m_targetTime);
//    ESP_LOGI("NewYearsEve", "initTargetTime: %s", s_targetTime);
//    std::cout << "initTargetTime: " << s_targetTime << std::endl;
}

int NewYearsEve::secondsUntilNewYears()
{
    std::time_t targetTime = std::mktime(&m_targetTime);
    std::time_t now = std::time(nullptr);
    return std::ceil(std::difftime(targetTime, now));
}

void NewYearsEve::initCurtain()
{
//    std::vector<std::string> curtain;
//    for (int i = curtainTime; i > 0; --i) {
//        curtain.push_back();
//    }

    m_fancyAnimation->setScript(
            {
                "ExpLet,0,9",
                "ExpLet,0,8",
                "ExpLet,0,7",
                "ExpLet,0,6",
                "ExpLet,0,5",
                "ExpLet,0,4",
                "ExpLet,0,3",
                "ExpLet,0,2",
                "ExpLet,0,1",
                });
}

void NewYearsEve::initFancyAnimation()
{
    m_fancyAnimation->setScript(
            {
                "ExpLet,0,2022",
                "Sprinkle",
                "ExpLet,1,2022",
                "ColorRays,1",
                "ColorRays",
                "ExpLet,0,2022",
                "BarsGrow",
                "BarsOpen",
                "ExpLet,1,2022",
                "BarsOpen,1",
                "BarsGrow,1",
                "ExpLet,0,2022",
                "CubeGrow,1",
                "CubeRoll",
                "ColorRays",
                "CircleGrow",
                "ColorRays,1",
                "CircleGrow,1",
                "CubeRoll,1",
                "BarsGrow,2",
                "BarsOpen,2",
                "CircleGrow,1",
                "CircleGrow,1",
                "ExpLet,1,2022",
                "BarsOpen,3",
                "BarsGrow,3",
                "CN",
            });
}

}