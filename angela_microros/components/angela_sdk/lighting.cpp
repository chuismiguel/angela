#include "lighting.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// log
#include "esp_log.h"

static const char *TAG = "LIGHTING";

Lights::Lights(gpio_num_t LightPin, uint32_t PixelN)
    : LightPin_(LightPin),
      PixelN_(PixelN),
      Strip_(nullptr)
{
}

esp_err_t Lights::Begin()
{
    led_strip_config_t StripConfig = {
        .strip_gpio_num = LightPin_,
        .max_leds = PixelN_,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_RGB,
        .flags = {
            .invert_out = false,
        }
    };

    led_strip_rmt_config_t RmtConfig = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000,
        .mem_block_symbols = 64,
        .flags = {
            .with_dma = true,
        }
    };

    ESP_ERROR_CHECK(
        led_strip_new_rmt_device(
            &StripConfig,
            &RmtConfig,
            &Strip_
        )
    );

    ESP_LOGI(TAG, "Configured LED strip with led n: %ld on pin %d!", PixelN_, LightPin_);

    return led_strip_clear(Strip_);
}

uint32_t Lights::FloatToColor(float Value)
{
    if (Value < 0.0f)
        Value = 0.0f;

    if (Value > 1.0f)
        Value = 1.0f;

    return static_cast<uint32_t>(Value * 255.0f);
}

esp_err_t Lights::SetLight(float R, float G, float B)
{
    for (int i = 0; i < static_cast<int>(PixelN_); i++)
    {
        ESP_ERROR_CHECK(
            led_strip_set_pixel(
                Strip_,
                i,
                FloatToColor(R),
                FloatToColor(G),
                FloatToColor(B)
            )
        );
    }

    return led_strip_refresh(Strip_);
}

esp_err_t Lights::SetLight(uint32_t PixelIdx, float R, float G, float B)
{
    ESP_ERROR_CHECK(
        led_strip_set_pixel(
            Strip_,
            PixelIdx,
            FloatToColor(R),
            FloatToColor(G),
            FloatToColor(B)
        )
    );

    return led_strip_refresh(Strip_);
}

void Lights::ShowSuccess()
{
    ESP_LOGI(TAG, "Success !");

    SetLight(0.0f, 1.0f, 0.0f);

    vTaskDelay(pdMS_TO_TICKS(500));

    SetLight(0.0f, 0.0f, 0.0f);
}

void Lights::WarnLight()
{
    ESP_LOGI(TAG, "Warning !");

    SetLight(1.0f, 1.0f, 0.0f);

    vTaskDelay(pdMS_TO_TICKS(200));

    SetLight(0.0f, 0.0f, 0.0f);
}

void Lights::ShowErrorLoop()
{
    while (true)
    {   
        ESP_LOGI(TAG, "Locked in error loop !");

        SetLight(1.0f, 0.0f, 0.0f);

        vTaskDelay(pdMS_TO_TICKS(300));

        SetLight(0.0f, 0.0f, 0.0f);

        vTaskDelay(pdMS_TO_TICKS(300));
    }
}