#include "ModeTime.h"
#include "TimeFormatter.h"
#include <esp_log.h>
#include <cJSON.h>

namespace LedWall::Mode {

ModeTime::ModeTime(LedMatrix &matrix): ModeText(matrix), m_updateDelay(1000)
{
    setScrollMode(ScrollNone);
}

bool ModeTime::update()
{
    if (!m_updateDelay.isTimedOut()) {
        return false;
    }

    static bool isOdd = false;
    isOdd = !isOdd;

    switch (getVariant()) {
        case VariantTime:
            showTime(isOdd);
            break;

        case VariantCountDown:
        case VariantCountDownTo:
            showCountDown(isOdd);
            break;

        case VariantCountUp:
        case VariantCountSince:
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
        switch (getVariant()) {
            case VariantCountDown:
            case VariantCountUp:
                setVariantParameter(requestedVariantParameter->valueint);
                break;

            case VariantCountDownTo:
                setCountDownToParameter(requestedVariantParameter->valuestring);
                break;

            case VariantCountSince:
                setCountSinceParameter(requestedVariantParameter->valuestring);
                break;

            default:
                // ignore parameter, variant does not accept parameters
                setVariantParameter(0);
                break;
        }
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

void ModeTime::showTime(bool includeColon)
{
    std::time_t now = std::time(nullptr);

    std::tm timeinfo{};
    localtime_r(&now, &timeinfo);

    char strftime_buf[64];
    strftime(strftime_buf, sizeof(strftime_buf), (includeColon ? "%H:%M" : "%H %M"), &timeinfo);

    setText(strftime_buf);
}

void ModeTime::showCountDown(bool includeColon)
{
    /**
     * TODO
     * less than 60 seconds? show only seconds, centered
     * more than 60/99 minutes? show HH:MM
     * more than 24h? show "~2 d", "~99 d"
     * more than 99d? "8" rotated by 90 degrees, infinity! :)
     */

    std::string remainingTime = formatSeconds(m_variantParameter, includeColon);

    if (m_variantParameter > 0) {
        --m_variantParameter;
    }

    setText(remainingTime);
}

void ModeTime::showCountUp(bool includeColon)
{
    std::string timeSoFar = formatSeconds(m_variantParameter, includeColon);

    ++m_variantParameter;

    setText(timeSoFar);
}

std::string ModeTime::formatSeconds(int secondsToFormat, bool includeColon)
{
    return TimeFormatter::formatSeconds(secondsToFormat, includeColon);
}

void ModeTime::setCountDownToParameter(const char *dateTimeString)
{
    std::time_t targetTime = parseDateTimeParameter(dateTimeString);
    if (targetTime == -1) {
        ESP_LOGW("ModeTime", "Failed to parse date time parameter");
        return;
    }

    std::time_t now = std::time(nullptr);
    int timeDiff = std::ceil(std::difftime(targetTime, now));

    ESP_LOGI("ModeTime", "setCountDownToParameter(%s), remaining: %ds", dateTimeString, timeDiff);
    setVariantParameter(timeDiff < 0 ? 0 : timeDiff);
}

void ModeTime::setCountSinceParameter(const char *dateTimeString)
{
    std::time_t targetTime = parseDateTimeParameter(dateTimeString);
    if (targetTime == -1) {
        ESP_LOGW("ModeTime", "Failed to parse date time parameter");
        return;
    }

    std::time_t now = std::time(nullptr);
    int timeDiff = std::ceil(std::difftime(now, targetTime));

    setVariantParameter(timeDiff < 0 ? 0 : timeDiff);
}

std::time_t ModeTime::parseDateTimeParameter(const char *dateTimeString)
{
    // (1) accepted date format: "yyyy-mm-dd HH:mm:ss"
    // (2) accepted date format: "yyyy-mm-dd" -> time: 00:00:00

    // FIXME possible issue: if target time is one-second-ago, parser will return -1 -> same as error value :(

    std::time_t targetTime = parseDateTime(dateTimeString);
    if (targetTime != -1) {
        return targetTime;
    }

    targetTime = parseDate(dateTimeString);
    if (targetTime != -1) {
        return targetTime;
    }

    return -1;
}

std::time_t ModeTime::parseDateTime(const char *dateTimeString)
{
    std::tm dateTime{};
    if (strptime(dateTimeString, "%Y-%m-%d %H:%M:%S", &dateTime) != nullptr) {
        dateTime.tm_isdst = -1; // detect/guess whether we need daylight saving
        std::time_t unixTime = std::mktime(&dateTime);
        if (unixTime != -1) {
            // we have time!
        }

        return unixTime;
    }

    return -1;
}

std::time_t ModeTime::parseDate(const char *dateTimeString)
{
    std::tm dateTime{};
    if (strptime(dateTimeString, "%Y-%m-%d", &dateTime) != nullptr) {
        dateTime.tm_hour = 0;
        dateTime.tm_min = 0;
        dateTime.tm_sec = 0;
        dateTime.tm_isdst = -1; // detect/guess whether we need daylight saving
        std::time_t unixTime = std::mktime(&dateTime);
        if (unixTime != -1) {
            // we have time!
        }

        return unixTime;
    }

    return -1;
}

}
