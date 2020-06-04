#ifndef LEDWALL_LEDMODEHSIBOY_H
#define LEDWALL_LEDMODEHSIBOY_H

#include "FastLED.h"
#include "LedMode.h"

// animations from https://gist.github.com/hsiboy/f9ef711418b40e259b06
//	FUNCTION-BASED ANIMATION SEQUENCES FOR WS2812 LED STRIPS
//	Using FastLED library
//  	Author: Dave Morris:	http://www.google.com/+DaveMorris128
//	Version 1.0 (2014-07-31)
//
//
//  	The following code includes "primitive animations" which are the base effect and
//	"aggregate animations" which are combinations of one or more primitive animations
//	Feel free to combine different primitives each loop for synergistic results but:
//		-If using an aggregate animation make sure your primatives don't clear the buffer each frame (FastLED.clear())
//		otherwise one animation will clear out any upstream animations before the loop sends the frame (FastLED.show())
//	This code is designed to handle multiple LED strips, each with its own animation....
//  e.g.
//		void loop()
//		{
//			Ring(stripA, frame, 30);
//			Spark(stripB, frame, 245);
//			FastLED.show();
//			frame += animateSpeed;
//		}

namespace LedWall {
namespace Mode {

class LedModeHsiboy: public LedMode
{
public:
    using LedMode::LedMode;

    bool update() override;

    void readOptions(cJSON *root) override;

    bool writeOptions(cJSON *root) override;

protected:
    uint16_t m_frame = 0;
    //Number of frames to increment per loop
    uint16_t m_animateSpeed = 255;
    //Active animation
    uint8_t m_animation = 9;
    //Global brightness percentage
    uint8_t m_brightness = 50;

    //3 chaser animations offset by 120 degrees each
    void TripleBounce(uint16_t frame);
    //2 chaser animations offset 180 degrees
    void DoubleChaser(uint16_t frame);
    //2 rings animations at inverse phases
    void RingPair(uint16_t frame);
    //Color spark where hue is function of frame
    void RainbowSpark(uint16_t frame, uint8_t fade);

    void Bounce(uint16_t frame, uint8_t hue);
    void Ring(uint16_t frame, uint8_t hue);
    void Wave(uint16_t frame, uint8_t hue);
    void WaveInt(uint16_t frame, uint8_t hue);
    void Spark(uint8_t fade, uint8_t hue);
    void Spark(uint8_t fade);
    void drawFractionalBar(int pos16, int width, uint8_t hue, bool wrap);
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_LEDMODEHSIBOY_H
