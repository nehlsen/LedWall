#ifndef LEDWALLSTUDIO_PARTARGUMENTS_H
#define LEDWALLSTUDIO_PARTARGUMENTS_H

#include <string>
#include <vector>

namespace LedWall::Mode::FancyParts {

class PartArguments
{
public:
    explicit PartArguments(std::string arguments);

    size_t argumentCount() const;

    std::string stringArgument(size_t index, const std::string& fallback) const;
    int intArgument(size_t index, int fallback) const;

    std::string argument(size_t index, const std::string& fallback) const;
    int argument(size_t index, int fallback) const;

private:
    void initializeArguments();

    std::string m_raw;
    std::vector<std::string> m_arguments;
};

}

#endif //LEDWALLSTUDIO_PARTARGUMENTS_H
