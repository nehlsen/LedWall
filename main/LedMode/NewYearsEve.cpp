#include "NewYearsEve.h"
#include "TimeFormatter.h"
#include <algorithm>
#include <esp_log.h>
#include "FancyDemo.h"
#include "FancyDemo/Nothing.h"
#include "FancyDemo/FdBarsGrow.h"
#include "FancyDemo/FdBarsOpen.h"
#include "FancyDemo/FdCubeRoll.h"
#include "FancyDemo/FdCubeGrow.h"
#include "FancyDemo/FdColorRays.h"
#include "FancyDemo/FdSprinkle.h"
#include "FancyDemo/FdExplodingLetters.h"
#include "FancyDemo/FdCircleGrow.h"
#include "FancyDemo/FdNHeartC.h"

//#include <iostream>

namespace LedWall::Mode {

static const uint8_t curtainTime = 9;

NewYearsEve::NewYearsEve(LedMatrix &matrix):
    ModeText(matrix)
{
    setScrollMode(ScrollNone);
    initTargetTime();
}

bool NewYearsEve::update()
{
    int remainingSeconds = secondsUntilNewYears();
    if (remainingSeconds > curtainTime) {
        return renderCountdown(remainingSeconds);
    } else if (remainingSeconds >= 1 && m_parts.size() < 5) { // 5 ?! assume 5 or less parts are the curtain and not the final animation
        m_parts.push_back(new Nothing(0, m_matrix));

        for (int i = curtainTime; i > 0; --i) {
            m_parts.push_back(new FdExplodingLetters(m_parts.back(), std::to_string(i)));
        }

    } else if (remainingSeconds < 1 && m_parts.size() < 15) { // 15 ?! assume final animation has at least 15 parts
        initParts();
    }

    return playParts();
}

bool NewYearsEve::playParts()
{
    m_matrix.clear(false); // FIXME remove. let the part decide to clear
    auto part = getCurrentPart();
    part->render(m_frame - part->getFirstFrame());
    m_frame++;

    return true;
}

FancyDemoPart * NewYearsEve::getCurrentPart()
{
    auto findCurrentPart = [this]() {
        return std::find_if(m_parts.begin(), m_parts.end(), [this](FancyDemoPart *part) {
            return m_frame >= part->getFirstFrame() && m_frame <= part->getLastFrame();
        });
    };

    auto it = findCurrentPart();
    if (it == m_parts.end()) {
        m_frame = 0;
        it = findCurrentPart();
    }

    if (it != m_parts.end()) {
        return *it;
    }

    return nullptr;
}

void NewYearsEve::initParts()
{
    m_parts.clear();
    m_parts.push_back(new Nothing(0, m_matrix));

    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "2021"));
    m_parts.push_back(new FdSprinkle(m_parts.back()));
    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "2021", REVERSE));
    m_parts.push_back(new FdColorRays(m_parts.back(), REVERSE));
    m_parts.push_back(new FdColorRays(m_parts.back()));
    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "2021"));
    m_parts.push_back(new FdBarsGrow(m_parts.back()));
    m_parts.push_back(new FdBarsOpen(m_parts.back()));
    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "2021", REVERSE));
    m_parts.push_back(new FdBarsOpen(m_parts.back(), REVERSE));
    m_parts.push_back(new FdBarsGrow(m_parts.back(), REVERSE));
    m_parts.push_back(new FdCubeGrow(m_parts.back()));
    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "2021"));
    m_parts.push_back(new FdCubeGrow(m_parts.back(), REVERSE));
    m_parts.push_back(new FdCubeRoll(m_parts.back()));
    m_parts.push_back(new FdColorRays(m_parts.back()));
    m_parts.push_back(new FdCircleGrow(m_parts.back()));
    m_parts.push_back(new FdColorRays(m_parts.back(), REVERSE));
    m_parts.push_back(new FdCircleGrow(m_parts.back(), REVERSE));
    m_parts.push_back(new FdCubeRoll(m_parts.back(), REVERSE));
    m_parts.push_back(new FdBarsGrow(m_parts.back(), VERTICAL));
    m_parts.push_back(new FdBarsOpen(m_parts.back(), VERTICAL));
    m_parts.push_back(new FdCircleGrow(m_parts.back(), REVERSE));
    m_parts.push_back(new FdCircleGrow(m_parts.back(), REVERSE));
    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "2021", REVERSE));
    m_parts.push_back(new FdBarsOpen(m_parts.back(), PartOptions(VERTICAL | REVERSE)));
    m_parts.push_back(new FdBarsGrow(m_parts.back(), PartOptions(VERTICAL | REVERSE)));
    m_parts.push_back(new FdNHeartC(m_parts.back()));
}

bool NewYearsEve::renderCountdown(int remainingSeconds)
{
    setText(TimeFormatter::formatSeconds(remainingSeconds, remainingSeconds%2));
    return ModeText::update();
}

void NewYearsEve::initTargetTime()
{
    std::time_t tnow = std::time(nullptr);
    std::tm *tmnow = std::localtime(&tnow);
    memcpy(&m_targetTime, tmnow, sizeof(std::tm));

    if (true) {
//        m_targetTime.tm_mday += 1;
//        m_targetTime.tm_hour += 1;
//        m_targetTime.tm_min += 1;
        m_targetTime.tm_sec += 18;
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

}