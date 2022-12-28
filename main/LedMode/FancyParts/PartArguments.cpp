#include <sstream>
#include <utility>
#include "PartArguments.h"

namespace LedWall::Mode::FancyParts {

PartArguments::PartArguments(std::string arguments):
    m_raw(std::move(arguments))
{
    initializeArguments();
}

size_t PartArguments::argumentCount() const
{
    return m_arguments.size();
}

std::string PartArguments::stringArgument(size_t index, const std::string &fallback) const
{
    if (index >= argumentCount()) {
        return fallback;
    }

    return m_arguments.at(index);
}

int PartArguments::intArgument(size_t index, int fallback) const
{
    if (index >= argumentCount()) {
        return fallback;
    }

    return std::stoi(m_arguments.at(index));
}

std::string PartArguments::argument(size_t index, const std::string &fallback) const
{
    return stringArgument(index, fallback);
}

int PartArguments::argument(size_t index, int fallback) const
{
    return intArgument(index, fallback);
}

void PartArguments::initializeArguments()
{
    std::string arg_n;
    std::stringstream argumentStream(m_raw);
    while (getline(argumentStream, arg_n, ',')) {
        m_arguments.push_back(arg_n);
    }
}

}
