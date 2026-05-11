#pragma once 

#include <Adafruit_NeoPixel.h>

class Lights
{
public:
    Lights(int lightPin, int pixelN);

    void begin();

    void setLight(float r, float g, float b);
    void setLight(int pixelIdx, float r, float g, float b);

    // greenish display for success showing
    void showSuccess();

    // yellow blink for warnings
    void warnLight();

    // red loop for critical errors
    void showErrorLoop();

private:
    uint8_t floatToColor(float value);

private:
    int lightPin_;
    int pixelN_;

    Adafruit_NeoPixel strip_;

};