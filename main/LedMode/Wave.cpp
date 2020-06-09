#include <cJSON.h>
#include "Wave.h"

namespace LedWall {
namespace Mode {

Wave::Wave(LedMatrix &matrix) : LedMode(matrix)
{
//    setWaveMode(WaveModeRadialRect);
    setWaveMode(WaveModeHorizontal);
    setWaveDirection(WaveDirectionReverse);

//    setWaveLength(15);
//    setWaveLength(m_matrix.getWidth()*2);
    setWaveLength(25);

//    setSpeed(15);  // travel  25% wavelength per second
    setSpeed(50);  // travel  50% wavelength per second
//    setSpeed(100); // travel 100% wavelength per second
//    setSpeed(200); // travel 200% wavelength per second

//    setModValue(80, 200);
//    setModValue(200, 80);

    // TODO direction? horiz/vert? angled?
    //   radial? circles/boxes from center?

    setModHue(0, 255);
    setModSaturation(255, 0);
    setModValue(75, 150);
}

void Wave::readOptions(cJSON *root)
{
    cJSON_AddNumberToObject(root, "waveMode", getWaveMode());
    cJSON_AddNumberToObject(root, "waveDirection", getWaveDirection());
    cJSON_AddNumberToObject(root, "waveLength", getWaveLength());
    cJSON_AddNumberToObject(root, "speed", getSpeed());
    cJSON_AddNumberToObject(root, "colorHueLow", m_colorHueLow);
    cJSON_AddNumberToObject(root, "colorHueHigh", m_colorHueHigh);
    cJSON_AddNumberToObject(root, "colorSaturationLow", m_colorSaturationLow);
    cJSON_AddNumberToObject(root, "colorSaturationHigh", m_colorSaturationHigh);
    cJSON_AddNumberToObject(root, "colorValueLow", m_colorValueLow);
    cJSON_AddNumberToObject(root, "colorValueHigh", m_colorValueHigh);
}

bool Wave::writeOptions(cJSON *root)
{
    bool changed = false;

    cJSON *objectItem = cJSON_GetObjectItem(root, "waveMode");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        setWaveMode(static_cast<Wave::WaveMode>(objectItem->valueint));
        changed = true;
    }

    objectItem = cJSON_GetObjectItem(root, "waveDirection");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        setWaveDirection(static_cast<Wave::WaveDirection>(objectItem->valueint));
        changed = true;
    }

    objectItem = cJSON_GetObjectItem(root, "waveLength");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        setWaveLength(objectItem->valueint);
        changed = true;
    }

    objectItem = cJSON_GetObjectItem(root, "speed");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        setSpeed(objectItem->valueint);
        changed = true;
    }

    objectItem = cJSON_GetObjectItem(root, "colorHueLow");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        setModHue(objectItem->valueint, m_colorHueHigh);
        changed = true;
    }

    objectItem = cJSON_GetObjectItem(root, "colorHueHigh");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        setModHue(m_colorHueLow, objectItem->valueint);
        changed = true;
    }

    objectItem = cJSON_GetObjectItem(root, "colorSaturationLow");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        setModSaturation(objectItem->valueint, m_colorSaturationHigh);
        changed = true;
    }

    objectItem = cJSON_GetObjectItem(root, "colorSaturationHigh");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        setModSaturation(m_colorSaturationLow, objectItem->valueint);
        changed = true;
    }

    objectItem = cJSON_GetObjectItem(root, "colorValueLow");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        setModValue(objectItem->valueint, m_colorValueHigh);
        changed = true;
    }

    objectItem = cJSON_GetObjectItem(root, "colorValueHigh");
    if (objectItem && cJSON_IsNumber(objectItem)) {
        setModValue(m_colorValueLow, objectItem->valueint);
        changed = true;
    }

    return changed;
}

Wave::WaveMode Wave::getWaveMode() const
{
    return m_waveMode;
}

void Wave::setWaveMode(Wave::WaveMode waveMode)
{
    if (m_waveMode == waveMode) {
        return;
    }

    m_waveMode = waveMode;
    switch (m_waveMode) {
        case WaveModeRadialCircle:
            initCircles();
            break;
        case WaveModeRadialRect:
            initRects();
            break;

        default:
            clearCircles();
            clearRects();
            break;
    }
}

Wave::WaveDirection Wave::getWaveDirection() const
{
    return m_waveDirection;
}

void Wave::setWaveDirection(Wave::WaveDirection waveDirection)
{
    m_waveDirection = waveDirection;
}

int Wave::getWaveLength() const
{
    return m_waveLength;
}

void Wave::setWaveLength(int waveLength)
{
    m_waveLength = waveLength;
    m_totalFrames = m_waveLength * framesPerSecond();
}

int Wave::getSpeed() const
{
    return m_speed;
}

void Wave::setSpeed(int speed)
{
    m_speed = speed;
}

std::pair<uint8_t, uint8_t> Wave::getModHue() const
{
    return {m_colorHueLow, m_colorHueHigh};
}

void Wave::setModHue(uint8_t low, uint8_t high)
{
    m_colorHueLow = low;
    m_colorHueHigh = high;
}

std::pair<uint8_t, uint8_t> Wave::getModSaturation() const
{
    return {m_colorSaturationLow, m_colorSaturationHigh};
}

void Wave::setModSaturation(uint8_t low, uint8_t high)
{
    m_colorSaturationLow = low;
    m_colorSaturationHigh = high;
}

std::pair<uint8_t, uint8_t> Wave::getModValue() const
{
    return {m_colorValueLow, m_colorValueHigh};
}

void Wave::setModValue(uint8_t low, uint8_t high)
{
    m_colorValueLow = low;
    m_colorValueHigh = high;
}

bool Wave::update()
{
    double offset = (double)m_frame/(m_totalFrames*10) * m_waveLength;

    switch (m_waveMode) {
        default:
        case WaveModeHorizontal:
            drawWaveHorizontal(offset);
            break;
        case WaveModeVertical:
            drawWaveVertical(offset);
            break;

        case WaveModeRadialCircle:
            drawCircles(offset);
            break;
        case WaveModeRadialRect:
            drawRects(offset);
            break;
    }

    m_frame += (m_waveLength * m_speed)/10;
    if (m_frame >= m_totalFrames*10) m_frame = 1;

    return true;
}

double Wave::sineValue(int x, double offset) const
{
    double value = sin(PI * (x - offset) / (m_waveLength/2.0));
    value = (value + 1) * 0.5; // shift wave up by one, squeeze wave between 0.0 and 1.0

    return value;
}

void Wave::drawWaveHorizontal(double offset)
{
    for (int x = 0; x < m_matrix.getWidth(); ++x) {
        const double value = sineValue(
                getWaveDirection() == WaveDirectionForward ? x : m_matrix.getWidth()-1-x,
                offset
                );

        const CHSV color(
            factoredConstrain(m_colorHueLow, m_colorHueHigh, value),
            factoredConstrain(m_colorSaturationLow, m_colorSaturationHigh, value),
            factoredConstrain(m_colorValueLow, m_colorValueHigh, value)
        );

        for (int y = 0; y < m_matrix.getHeight(); ++y) {
            m_matrix.pixel(x, y) = color;
        }
    }
}

void Wave::drawWaveVertical(double offset)
{
    for (int y = 0; y < m_matrix.getHeight(); ++y) {
        const double value = sineValue(
                getWaveDirection() == WaveDirectionForward ? y : m_matrix.getWidth()-1-y,
                offset
        );

        const CHSV color(
                factoredConstrain(m_colorHueLow, m_colorHueHigh, value),
                factoredConstrain(m_colorSaturationLow, m_colorSaturationHigh, value),
                factoredConstrain(m_colorValueLow, m_colorValueHigh, value)
        );

        for (int x = 0; x < m_matrix.getWidth(); ++x) {
            m_matrix.pixel(x, y) = color;
        }
    }
}

void Wave::drawCircles(double offset)
{
    for (int n = 0; n < m_circles.size(); ++n) {
        const double value = sineValue(
                getWaveDirection() == WaveDirectionForward ? n : m_circles.size()-1-n,
                offset
        );

        const CHSV color(
                factoredConstrain(m_colorHueLow, m_colorHueHigh, value),
                factoredConstrain(m_colorSaturationLow, m_colorSaturationHigh, value),
                factoredConstrain(m_colorValueLow, m_colorValueHigh, value)
        );

        m_circles[n].setBorderColor(color).render(m_matrix);
    }
}

void Wave::initCircles()
{
    const int x = m_matrix.getWidth() / 2 + (m_matrix.getWidth()%2 -1);
    const int y = m_matrix.getHeight() / 2 + (m_matrix.getHeight()%2 -1);
    const int dropRadius = (x > y ? x : y) + 1;
    const int distance = 1;

    m_circles.resize(dropRadius/distance);
    for (int n = 0; n < m_circles.size(); ++n) {
        m_circles[n].setCenter(x, y).setRadius((n+1)*distance);
    }
}

void Wave::clearCircles()
{
    m_circles.clear();
}

void Wave::drawRects(double offset)
{
    for (int n = 0; n < m_rects.size(); ++n) {
        const double value = sineValue(
                getWaveDirection() == WaveDirectionForward ? n : m_rects.size()-1-n,
                offset
                );

        const CHSV color(
                factoredConstrain(m_colorHueLow, m_colorHueHigh, value),
                factoredConstrain(m_colorSaturationLow, m_colorSaturationHigh, value),
                factoredConstrain(m_colorValueLow, m_colorValueHigh, value)
        );

        m_rects[n].setBorderColor(color).render(m_matrix);
    }
}

void Wave::initRects()
{
    uint8_t width = m_matrix.getWidth();
    uint8_t height = m_matrix.getHeight();

    m_rects.resize((width > height ? height : width)/2);

    for (int n = 0; ; ++n) {
        m_rects[n] = Rect(n, n, width, height);

        if (width > 2 && height > 2) {
            width -= 2;
            height -= 2;
        } else {
            break;
        }
    }
}

void Wave::clearRects()
{
    m_rects.clear();
}

uint8_t Wave::factoredConstrain(uint8_t low, uint8_t high, double factor)
{
    if (low == high) {
        return low;
    }

    if (factor >= 1.0) factor = 1.0;
    if (factor <= 0.0) factor = 0.0;

    if (low > high) {
        int t = low;
        low = high;
        high = t;
        factor = 1.0 - factor;
    }

    if (factor >= 1.0) return high;
    if (factor <= 0.0) return low;

    return (high - low) * factor + low;
}

} // namespace Mode
} // namespace LedWall
