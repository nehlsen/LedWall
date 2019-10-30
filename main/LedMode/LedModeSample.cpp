#include "LedModeSample.h"

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p =
        {
                CRGB::Red,
                CRGB::Gray, // 'white' is too bright compared to red and blue
                CRGB::Blue,
                CRGB::Black,

                CRGB::Red,
                CRGB::Gray,
                CRGB::Blue,
                CRGB::Black,

                CRGB::Red,
                CRGB::Red,
                CRGB::Gray,
                CRGB::Gray,
                CRGB::Blue,
                CRGB::Blue,
                CRGB::Black,
                CRGB::Black
        };

void LedModeSample::update()
{
    changePalettePeriodically();

    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */

    for( int i = 0; i < m_ledCount; i++) {
        m_leds[i] = ColorFromPalette(m_currentPalette, startIndex, 64, m_currentBlending);
        startIndex += 3;
    }
}

void LedModeSample::changePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;

    if(lastSecond != secondHand) {
        lastSecond = secondHand;
        if(secondHand ==  0) { m_currentPalette = RainbowColors_p;         m_currentBlending = LINEARBLEND; }
        if(secondHand == 10) { m_currentPalette = RainbowStripeColors_p;   m_currentBlending = NOBLEND;  }
        if(secondHand == 15) { m_currentPalette = RainbowStripeColors_p;   m_currentBlending = LINEARBLEND; }
        if(secondHand == 20) { setupPurpleAndGreenPalette();               m_currentBlending = LINEARBLEND; }
        if(secondHand == 25) { setupTotallyRandomPalette();                m_currentBlending = LINEARBLEND; }
        if(secondHand == 30) { setupBlackAndWhiteStripedPalette();         m_currentBlending = NOBLEND; }
        if(secondHand == 35) { setupBlackAndWhiteStripedPalette();         m_currentBlending = LINEARBLEND; }
        if(secondHand == 40) { m_currentPalette = CloudColors_p;           m_currentBlending = LINEARBLEND; }
        if(secondHand == 45) { m_currentPalette = PartyColors_p;           m_currentBlending = LINEARBLEND; }
        if(secondHand == 50) { m_currentPalette = myRedWhiteBluePalette_p; m_currentBlending = NOBLEND;  }
        if(secondHand == 55) { m_currentPalette = myRedWhiteBluePalette_p; m_currentBlending = LINEARBLEND; }
    }
}

void LedModeSample::setupTotallyRandomPalette()
{
    for(int i = 0; i < 16; i++) {
        m_currentPalette[i] = CHSV(random8(), 255, random8());
    }
}

void LedModeSample::setupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( m_currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    m_currentPalette[0] = CRGB::White;
    m_currentPalette[4] = CRGB::White;
    m_currentPalette[8] = CRGB::White;
    m_currentPalette[12] = CRGB::White;
}

void LedModeSample::setupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;

    m_currentPalette = CRGBPalette16(
            green,  green,  black,  black,
            purple, purple, black,  black,
            green,  green,  black,  black,
            purple, purple, black,  black );
}
