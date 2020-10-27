#include <cJSON.h>
//#include <Delayer.h>
#include "ModeTime.h"

namespace LedWall::Mode {

ModeTime::ModeTime(LedMatrix &matrix): ModeText(matrix)
{
    setScrollMode(ScrollNone);
}

bool ModeTime::update()
{
//    EBLi::Delayer delayer;

    const uint16_t delay = 1000;
    int64_t currentTime = esp_timer_get_time() / 1000;
    if ((currentTime - m_lastTimeUpdate < delay) && m_lastTimeUpdate > 0) {
        return false;
    }
    m_lastTimeUpdate = currentTime;

    static bool isOdd = false;
    isOdd = !isOdd;

    switch (getVariant()) {
        case VariantTime:
            showTime(isOdd);
            break;

        case VariantCountDown:
            showCountDown(isOdd);
            break;

        case VariantCountUp:
            showCountUp(isOdd);
            break;

        default:
            // TODO
            break;
    }

    return ModeText::update();
}

void ModeTime::readOptions(cJSON *root)
{
    cJSON_AddNumberToObject(root, "variant", getVariant());
    cJSON_AddNumberToObject(root, "parameter", getVariantParameter());
}

bool ModeTime::writeOptions(cJSON *root)
{
    cJSON *const requestedVariant = cJSON_GetObjectItem(root, "variant");
    if (requestedVariant) {
        setVariant(static_cast<Variant>(requestedVariant->valueint));
    }

    cJSON *const requestedVariantParameter = cJSON_GetObjectItem(root, "parameter");
    if (requestedVariantParameter) {
        setVariantParameter(requestedVariantParameter->valueint);
    }

    return requestedVariant || requestedVariantParameter;
}

void ModeTime::setVariant(ModeTime::Variant variant)
{
    m_variant = variant;
}

ModeTime::Variant ModeTime::getVariant() const
{
    return m_variant;
}

void ModeTime::setVariantParameter(int variantParameter)
{
    m_variantParameter = variantParameter;
}

int ModeTime::getVariantParameter() const
{
    return m_variantParameter;
}

void ModeTime::setVariantTime()
{
    setVariant(Variant::VariantTime);
}

void ModeTime::setVariantCountdown(int countdownSeconds)
{
    setVariant(Variant::VariantCountDown);
    setVariantParameter(countdownSeconds);
}

void ModeTime::setVariantCountUp()
{
    setVariant(Variant::VariantCountUp);
    setVariantParameter(0);
}

void ModeTime::showTime(bool isOddSecond)
{
    time_t now;
    char strftime_buf[64];
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), (isOddSecond ? "%H %M" : "%H:%M"), &timeinfo);

    setText(strftime_buf);
}

void ModeTime::showCountDown(bool isOddSecond)
{
    std::string remainingTime = formatSeconds(m_variantParameter, isOddSecond);

    if (m_variantParameter > 0) {
        --m_variantParameter;
    }

    setText(remainingTime);
}

void ModeTime::showCountUp(bool isOddSecond)
{
    std::string timeSoFar = formatSeconds(m_variantParameter, isOddSecond);

    ++m_variantParameter;

    setText(timeSoFar);
}

std::string ModeTime::formatSeconds(int secondsToFormat, bool includeSplittingDots)
{
    std::string formattedSeconds;

    int seconds = secondsToFormat % 60;
    int minutes = (secondsToFormat-seconds) / 60;
    // FIXME if minutes > 99

    formattedSeconds += (minutes < 10 ? "0" : "");
    formattedSeconds += std::to_string(minutes);

    formattedSeconds += (includeSplittingDots ? " " : ":");

    formattedSeconds += (seconds < 10 ? "0" : "");
    formattedSeconds += std::to_string(seconds);

    return formattedSeconds;
}

}
