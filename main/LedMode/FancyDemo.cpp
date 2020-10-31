#include <algorithm>
#include "FancyDemo.h"
#include "FancyDemo/FancyDemoPart1.h"
#include "FancyDemo/FancyDemoPart2.h"
#include "FancyDemo/FancyDemoPart3.h"
#include "FancyDemo/FancyDemoPart4.h"
#include "FancyDemo/FancyDemoPart5.h"
#include "FancyDemo/FancyDemoPart6.h"

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
    m_parts.push_back(new FancyDemoPart1(0, m_matrix));
    m_parts.push_back(new FancyDemoPart2(m_parts.back()));
    m_parts.push_back(new FancyDemoPart3(m_parts.back()));
    m_parts.push_back(new FancyDemoPart4(m_parts.back()));
    m_parts.push_back(new FancyDemoPart5(m_parts.back()));
    m_parts.push_back(new FancyDemoPart6(m_parts.back(), true));
    m_parts.push_back(new FancyDemoPart6(m_parts.back(), false));
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
