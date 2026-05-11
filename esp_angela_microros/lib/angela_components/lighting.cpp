#include "lighting.h"

Lights::Lights(int lightPin, int pixelN)
    : lightPin_(lightPin),
      pixelN_(pixelN),
      strip_(pixelN, lightPin, NEO_GRB + NEO_KHZ800)
{
}

void Lights::begin()
{
    strip_.begin();
    strip_.clear();
    strip_.show();
}

void Lights::setLight(float r, float g, float b)
{
    uint8_t Red = floatToColor(r);
    uint8_t Green = floatToColor(g);
    uint8_t Blue = floatToColor(b);

    for (int i = 0; i < pixelN_; i++)
    {
        strip_.setPixelColor(i, strip_.Color(Red, Green, Blue));
    }

    strip_.show();
}

void Lights::setLight(int PixelIdx, float r, float g, float b)
{
    if (PixelIdx < 0 || PixelIdx >= pixelN_)
    {
        return;
    }

    uint8_t Red = floatToColor(r);
    uint8_t Green = floatToColor(g);
    uint8_t Blue = floatToColor(b);

    strip_.setPixelColor(PixelIdx, strip_.Color(Red, Green, Blue));
    strip_.show();
}

void Lights::showSuccess()
{
    for (int i = 0; i < 3; i++)
    {
        setLight(0.0f, 1.0f, 0.0f); // Green for success
        delay(200);
        setLight(0.0f, 0.0f, 0.0f); // Off
        delay(200);
    }
}

void Lights::warnLight()
{
    for (int i = 0; i < 3; i++)
    {
        setLight(1.0f, 1.0f, 0.0f); // Yellow for warning
        delay(100);
        setLight(0.0f, 0.0f, 0.0f);
        delay(100);
    }
}

void Lights::showErrorLoop()
{
    while (true)
    {
        setLight(1.0f, 0.0f, 0.0f); // Red for error
        delay(1000);
        setLight(0.0f, 0.0f, 0.0f); // Off
        delay(1000);
    }
}

uint8_t Lights::floatToColor(float value)
{
    if (value < 0.0f)
    {
        value = 0.0f;
    }

    if (value > 1.0f)
    {
        value = 1.0f;
    }

    return static_cast<uint8_t>(value * 255.0f);
}