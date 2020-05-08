#include "Fire.h"
#include <cJSON.h>

Fire::Fire(LedMatrix& matrix):
    LedMode(matrix)
{
    m_heat = static_cast<uint8_t *>(calloc(m_matrix.getWidth() * m_matrix.getHeight(), sizeof(uint8_t)));
    if (m_heat == nullptr) {
        ESP_LOGE("LedMode:Fire", "Failed to allocate memory for heat map");
    }

    m_palette = CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);
}

Fire::~Fire()
{
    if (m_heat != nullptr) {
        free(m_heat);
    }
}

void Fire::update()
{
    if (!isReady()) {
        return;
    }

    for(uint8_t x = 0; x < m_matrix.getWidth(); ++x) {
        updateColumn(x);
    }
}

void Fire::readOptions(cJSON *root)
{
    cJSON_AddNumberToObject(root, "cooling", m_cooling);
    cJSON_AddNumberToObject(root, "sparking", m_sparking);
    // TODO implement changing the palette
}

bool Fire::writeOptions(cJSON *root)
{
    cJSON *const requestedCooling = cJSON_GetObjectItem(root, "cooling");
    if (requestedCooling) {
        m_cooling = constrain(requestedCooling->valueint, 0, 255);
    }

    cJSON *const requestedSparking = cJSON_GetObjectItem(root, "sparking");
    if (requestedSparking) {
        m_sparking = constrain(requestedSparking->valueint, 0, 255);
    }

    return requestedCooling || requestedSparking;
}

bool Fire::isReady() const
{
    return m_heat != nullptr;
}

uint8_t *Fire::heat(uint8_t x, uint8_t y)
{
    if (!isReady()) {
        return nullptr;
    }

    if (x >= m_matrix.getWidth() || y >= m_matrix.getHeight()) {
        return &m_heat[0];
    }

    return &m_heat[(y * m_matrix.getWidth() + x)];
}

void Fire::updateColumn(uint8_t x)
{
    if (!isReady()) {
        return;
    }

    // Step 1.  Cool down every cell a little
    for(int i = 0; i < m_matrix.getHeight(); i++) {
        *heat(x, i) = qsub8(*heat(x, i), random8(0, ((m_cooling * 10) / m_matrix.getHeight()) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for(int k = m_matrix.getHeight() - 1; k >= 2; k--) {
        *heat(x, k) = (*heat(x, k - 1) + *heat(x, k - 2) + *heat(x, k - 2)) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if(random8() < m_sparking) {
        int newSpark_y = random8(7);
        *heat(x, newSpark_y) = qadd8(*heat(x, newSpark_y), random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for(int y = 0; y < m_matrix.getHeight(); y++) {
        // Scale the heat value from 0-255 down to 0-240
        // for best results with color palettes.
        uint8_t colorindex = scale8(*heat(x, y), 240);
        m_matrix.pixel(x, y) = ColorFromPalette(m_palette, colorindex);
    }
}
