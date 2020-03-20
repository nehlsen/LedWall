#include <cJSON.h>
#include "LedModeHsiboy.h"

#define MAX_INT_VALUE 65536
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void LedModeHsiboy::update()
{
    //Each animation adjusts the "targetStrip" specified in its parameter.
    //Animations are a function of the current animation frame "frame"
    //Once you've applied the animation to the current strip/frame, Its up to the main loop to send the data to the strip(s)

    switch (m_animation) {
        case 0:
            RingPair(m_frame);
            break;
        case 1:
            DoubleChaser(m_frame);
            break;
        case 2:
            TripleBounce(m_frame);
            break;
        case 3:
            WaveInt(m_frame, 180);
            break;
        case 4:
            Wave(m_frame, 180);
            break;
        case 5:
            // Blue spark (Slow)
            // Overloaded version of "Spark" with Hue value, 255 for fade is the slowest fade possible. 256 is on/off
            Spark(255, 188);
            // Slow things down a bit more for Slow Spark
            vTaskDelay(2 / portTICK_PERIOD_MS);
            break;
        case 6:	//Blue spark (fast)
            Spark(246, 188);		//Overloaded version of "Spark" with Hue value, 246 fade is faster which makes for a sharper dropoff
            break;
        case 7:
            // White spark (Slow)
            // "Spark" function without hue make a white spark, 255 for fade is the slowest fade possible.
            Spark(255);
            // Slow things down a bit more for Slow Spark
            vTaskDelay(2 / portTICK_PERIOD_MS);
            break;
        case 8:
            // White spark (fast)
            // "Spark" function without hue make a white spark, 246 fade is faster which makes for a sharper dropoff
            Spark(245);
            break;
        case 9:
            RainbowSpark(m_frame, 240);		//240 for dropoff is a pretty sharp fade, good for this animation
            break;
        default:
            vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    m_frame += m_animateSpeed;
}

void LedModeHsiboy::readOptions(cJSON *root)
{
    cJSON_AddNumberToObject(root, "animateSpeed", m_animateSpeed);
    cJSON_AddNumberToObject(root, "animation", m_animation);
}

bool LedModeHsiboy::writeOptions(cJSON *root)
{
    cJSON *const animateSpeed = cJSON_GetObjectItem(root, "animateSpeed");
    if (animateSpeed) {
        int speed = animateSpeed->valueint;
        m_animateSpeed = speed < 0 ? 0 : speed > 255 ? 255 : speed;
    }

    cJSON *const animation = cJSON_GetObjectItem(root, "animation");
    if (animation) {
        int animationIndex = animation->valueint;
        m_animation = animationIndex < 0 ? 0 : animationIndex > 9 ? 9 : animationIndex;
    }

    return animateSpeed || animation;
}

//#######################################################################################################
//##                                      AGGREGATE ANIMATIONS                                         ##
//#######################################################################################################
void LedModeHsiboy::TripleBounce(uint16_t frame)
{
    FastLED.clear();
    Bounce(frame, 0);
    Bounce(frame + (MAX_INT_VALUE/3), 100);
    Bounce(frame + (MAX_INT_VALUE/3)*2, 150);
}

void LedModeHsiboy::DoubleChaser(uint16_t frame)
{
    FastLED.clear();
    frame = frame * 2;
    Ring(frame, 0);
    Ring(frame + (MAX_INT_VALUE / 2), 150);
}

void LedModeHsiboy::RingPair(uint16_t frame)
{
    FastLED.clear();
    Ring(frame, 30);
    Ring(MAX_INT_VALUE - frame, 150);
}

void LedModeHsiboy::RainbowSpark(uint16_t frame, uint8_t fade)
{
    Spark(fade, frame/255);
    delay(20);
}

//#######################################################################################################
//##                                PRIMATIVE ANIMATION FUNCTIONS                                      ##
//#######################################################################################################

//*********************     Bounce      ***************************
// Linear "Larson scanner" (or knight rider effect) with anti-aliasing
// Color is determined by "hue"
//*****************************************************************
void LedModeHsiboy::Bounce(uint16_t frame, uint8_t hue)
{
    uint16_t pos16;
    if (frame < (MAX_INT_VALUE / 2)) {
        pos16 = frame * 2;
    } else {
        pos16 = MAX_INT_VALUE - ((frame - (MAX_INT_VALUE/2))*2);
    }

    int position = map(pos16, 0, MAX_INT_VALUE, 0, ((FastLED.size()) * 16));
    drawFractionalBar(position, 3, hue,0);
}

//************************          Ring           ******************************
// Anti-aliased cyclical chaser, 3 pixels wide
// Color is determined by "hue"
//*****************************************************
void LedModeHsiboy::Ring(uint16_t frame, uint8_t hue)
{
    int pos16 = map(frame, 0, MAX_INT_VALUE, 0, ((FastLED.size()) * 16));
    drawFractionalBar(pos16, 3, hue,1);
}

//***************************   Wave [Float Math]  *******************************
// Squeezed sine wave
// Uses slow, Arduino sin() function
// Squeezing achieved by using an exponential (^8) sin value
// Color is determined by "hue"
//***********************************************************************************
void LedModeHsiboy::Wave(uint16_t frame, uint8_t hue)
{
    FastLED.clear();
    float deg;
    float value;
    for (uint8_t i=0; i < FastLED.size(); i++) {
        deg = float(frame + ((MAX_INT_VALUE/FastLED.size())*i)) / (float(MAX_INT_VALUE)) * 360.0;
        value = pow(sin(radians(deg)),8);    //Squeeeeeeze

        //Chop sine wave (no negative values)
        if (value >= 0) {
            FastLED.leds()[i] += CHSV(hue, 255, value*256);
        }
    }
}

//***************************   Wave [Integer Math]  *******************************
// unadulterated sine wave.
// Uses FastLED sin16() and no float math for efficiency.
// Since im stuck with integer values, exponential wave-forming is not possible (unless i'm wrong???)
// Color is determined by "hue"
//***********************************************************************************
void LedModeHsiboy::WaveInt(uint16_t frame, uint8_t hue)
{
    FastLED.clear();
    uint8_t value;
    for(uint8_t i=0; i<FastLED.size(); i++) {
        value = (sin16(frame+((MAX_INT_VALUE/FastLED.size())*i)) + (MAX_INT_VALUE/2))/256;
        if (value >= 0) {
            FastLED.leds()[i] += CHSV(hue,255,value);
        }
    }
}

//********************************   Color Spark  ***********************************
// Color of the sparks is determined by "hue"
// Frequency of sparks is determined by global var "animateSpeed"
// "animateSpeed" var contrained from 1 - 255 (0.4% - 100%)
// "fade" parameter specifies dropoff (next frame brightness = current frame brightness * (x/256)
// fade = 256 means no dropoff, pixels are on or off
// NOTE: this animation doesnt clear the previous buffer because the fade/dropoff is a function of the previous LED state
//***********************************************************************************
void LedModeHsiboy::Spark(uint8_t fade, uint8_t hue)
{
    uint16_t rand = random16();

    for (int i=0; i<FastLED.size(); i++) {
        FastLED.leds()[i].nscale8(fade);
    }

    if (rand < (MAX_INT_VALUE / (256 - (constrain(m_animateSpeed,1,256))))) {
        FastLED.leds()[rand % FastLED.size()].setHSV(hue,255,255);
    }
}

//******************************       Spark       **********************************
// Same as color spark but no hue value, // in HSV white is any hue with 0 saturation
// Frequency of sparks is a percentage mapped to global var "animateSpeed"
// "animateSpeed" var contrained from 1 - 255 (0.4% - 100%)
// "fade" parameter specifies dropoff (next frame brightness = current frame brightness * (x/256)
// fade = 256 means no dropoff, pixels are on or off
// NOTE: this animation doesnt clear the previous buffer because the fade/dropoff is a function of the previous LED state
//***********************************************************************************
void LedModeHsiboy::Spark(uint8_t fade)
{
    uint16_t rand = random16();

    for (int i=0; i<FastLED.size(); i++) {
        FastLED.leds()[i].nscale8(fade);
    }

    if (rand < (MAX_INT_VALUE / (256 - (constrain(m_animateSpeed,1,255))))) {
        FastLED.leds()[rand % FastLED.size()].setHSV(0,0,255);
    }
}

//Anti-aliasing code care of Mark Kriegsman Google+: https://plus.google.com/112916219338292742137/posts/2VYNQgD38Pw
void LedModeHsiboy::drawFractionalBar(int pos16, int width, uint8_t hue, bool wrap)
{
    uint8_t i = pos16 / 16; // convert from pos to raw pixel number

    uint8_t frac = pos16 & 0x0F; // extract the 'factional' part of the position
    uint8_t firstPixelBrightness = 255 - (frac * 16);

    uint8_t lastPixelBrightness = 255 - firstPixelBrightness;

    uint8_t bright;
    for (int n = 0; n <= width; n++) {
        if (n == 0) {
            // first pixel in the bar
            bright = firstPixelBrightness;
        } else if (n == width) {
            // last pixel in the bar
            bright = lastPixelBrightness;
        } else {
            // middle pixels
            bright = 255;
        }

        FastLED.leds()[i] += CHSV(hue, 255, bright );
        i++;
        if (i == FastLED.size()) {
            if (wrap == 1) {
                i = 0; // wrap around
            } else {
                return;
            }
        }
    }
}
