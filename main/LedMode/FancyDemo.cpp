#include <algorithm>
#include "FancyDemo.h"
#include "FancyDemo/FdBarsGrow.h"
#include "FancyDemo/FdBarsOpen.h"
#include "FancyDemo/FancyDemoPart3.h"
#include "FancyDemo/FancyDemoPart4.h"
#include "FancyDemo/FdRollingCube.h"
#include "FancyDemo/FdColorRays.h"
#include "FancyDemo/FdSprinkle.h"
#include "FancyDemo/FdExplodingLetters.h"

namespace LedWall::Mode {

FancyDemo::FancyDemo(LedMatrix &matrix):
    LedMode(matrix)
{
    initParts();
}

bool FancyDemo::update()
{
    m_matrix.clear(false); // FIXME remove. let the part decide to clear
//    m_matrix.fade(192);

    auto part = getCurrentPart();
    if (part == nullptr) {
        // FIXME WTF?!
    }
    part->render(m_frame - part->getFirstFrame());
    m_frame++;

    return true;
}

void FancyDemo::initParts()
{
//    m_parts.push_back(new FdExplodingLetters(0, m_matrix, "10"));
//    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "9"));
//    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "8"));
//    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "7"));
//    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "6"));
//    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "5"));
//    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "4"));
//    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "3"));
//    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "2"));
//    m_parts.push_back(new FdExplodingLetters(m_parts.back(), "1"));
//    m_parts.push_back(new FdBarsGrow(m_parts.back()));
//    m_parts.push_back(new FdBarsOpen(m_parts.back()));
//    m_parts.push_back(new FdBarsGrow(m_parts.back(), VERTICAL));
//    m_parts.push_back(new FdBarsOpen(m_parts.back(), VERTICAL));
//    m_parts.push_back(new FancyDemoPart3(m_parts.back()));
//    m_parts.push_back(new FancyDemoPart4(m_parts.back()));
//    m_parts.push_back(new FdRollingCube(m_parts.back(), true));
//    m_parts.push_back(new FdColorRays(m_parts.back(), true));
//    m_parts.push_back(new FdColorRays(m_parts.back(), false));
//    m_parts.push_back(new FdRollingCube(m_parts.back(), false));
//    m_parts.push_back(new FdSprinkle(m_parts.back()));


    m_parts.push_back(new FdBarsGrow(0, m_matrix));
    m_parts.push_back(new FdBarsOpen(m_parts.back()));
    m_parts.push_back(new FdBarsOpen(m_parts.back(), REVERSE));
    m_parts.push_back(new FdBarsGrow(m_parts.back(), REVERSE));
    m_parts.push_back(new FdBarsGrow(m_parts.back(), VERTICAL));
    m_parts.push_back(new FdBarsOpen(m_parts.back(), VERTICAL));
    m_parts.push_back(new FdBarsOpen(m_parts.back(), PartOptions(VERTICAL | REVERSE)));
    m_parts.push_back(new FdBarsGrow(m_parts.back(), PartOptions(VERTICAL | REVERSE)));
}

FancyDemoPart *FancyDemo::getCurrentPart()
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

}
