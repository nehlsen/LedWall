#include "FancyPart.h"

#include <utility>

namespace LedWall::Mode::FancyParts {

FancyPart::FancyPart(LedMatrix &matrix, PartOptions options, std::string arguments):
    m_matrix(matrix), m_options(options), m_arguments(std::move(arguments))
{}

void FancyPart::render(uint16_t relativeFrame)
{
    renderImpl(isForward() ? relativeFrame : getFrameCount() - relativeFrame);
}

bool FancyPart::hasOption(PartOptions option) const
{
    return m_options & option;
}

bool FancyPart::isForward() const
{
    return !hasOption(REVERSE);
}

bool FancyPart::isHorizontal() const
{
    return !hasOption(VERTICAL);
}

}
