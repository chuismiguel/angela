#pragma once

#include "led_strip.h"

class Lights
{
public:
    Lights(gpio_num_t LightPin, uint32_t PixelN);

    esp_err_t Begin();

    esp_err_t SetLight(float R, float G, float B);
    esp_err_t SetLight(uint32_t PixelIdx, float R, float G, float B);

    void ShowSuccess();
    void WarnLight();
    void ShowErrorLoop();

private:
    uint32_t FloatToColor(float Value);

private:
    gpio_num_t LightPin_;
    uint32_t PixelN_;

    led_strip_handle_t Strip_;
};