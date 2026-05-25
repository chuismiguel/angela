#include "movement.h"

#include <cmath>
#include <algorithm>

#include "driver/ledc.h"
#include "esp_err.h"

// log
#include "esp_log.h"

namespace
{
    constexpr uint32_t PWMFreq = 1000;
    constexpr ledc_timer_bit_t PWMResolution = LEDC_TIMER_8_BIT;
    constexpr uint32_t MaxDuty = 255;

    constexpr ledc_mode_t SpeedMode = LEDC_LOW_SPEED_MODE;
    constexpr ledc_timer_t TimerNum = LEDC_TIMER_0;

    inline float Clamp(float Value, float Min, float Max)
    {
        return std::max(Min, std::min(Value, Max));
    }

    inline uint32_t FloatAbs(float Value)
    {
        return static_cast<uint32_t>(std::fabs(Value));
    }
}

Movement::Movement(int Mot1Fwd, int Mot1Bwd, int Mot2Fwd, int Mot2Bwd)
{
    mot1FwdPin_ = static_cast<gpio_num_t>(Mot1Fwd);
    mot1BwdPin_ = static_cast<gpio_num_t>(Mot1Bwd);
    mot2FwdPin_ = static_cast<gpio_num_t>(Mot2Fwd);
    mot2BwdPin_ = static_cast<gpio_num_t>(Mot2Bwd);

    // change used channels to avoid conflicts with servos.
    mot1FwdCh_ = LEDC_CHANNEL_3;
    mot1BwdCh_ = LEDC_CHANNEL_4;
    mot2FwdCh_ = LEDC_CHANNEL_5;
    mot2BwdCh_ = LEDC_CHANNEL_6;
}

void Movement::begin()
{
    ledc_timer_config_t TimerConfig = {};
    TimerConfig.speed_mode = SpeedMode;
    TimerConfig.timer_num = TimerNum;
    TimerConfig.duty_resolution = PWMResolution;
    TimerConfig.freq_hz = PWMFreq;
    TimerConfig.clk_cfg = LEDC_AUTO_CLK;

    ESP_ERROR_CHECK(ledc_timer_config(&TimerConfig));

    ConfigureChannel(mot1FwdPin_, mot1FwdCh_);
    ConfigureChannel(mot1BwdPin_, mot1BwdCh_);
    ConfigureChannel(mot2FwdPin_, mot2FwdCh_);
    ConfigureChannel(mot2BwdPin_, mot2BwdCh_);

    stop();
}

void Movement::ConfigureChannel(gpio_num_t Pin, ledc_channel_t Channel)
{
    ledc_channel_config_t ChannelConfig = {};

    ChannelConfig.gpio_num = Pin;
    ChannelConfig.speed_mode = SpeedMode;
    ChannelConfig.channel = Channel;
    ChannelConfig.intr_type = LEDC_INTR_DISABLE;
    ChannelConfig.timer_sel = TimerNum;
    ChannelConfig.duty = 0;
    ChannelConfig.hpoint = 0;

    ESP_ERROR_CHECK(ledc_channel_config(&ChannelConfig));
}

void Movement::WriteDuty(ledc_channel_t Channel, uint32_t Duty)
{
    ESP_ERROR_CHECK(
        ledc_set_duty(SpeedMode, Channel, Duty));

    ESP_ERROR_CHECK(
        ledc_update_duty(SpeedMode, Channel));
}

void Movement::setMotor(ledc_channel_t FwdCh,
                        ledc_channel_t BwdCh,
                        float Power)
{
    Power = Clamp(Power, -1.0f, 1.0f);

    uint32_t Duty =
        static_cast<uint32_t>(std::fabs(Power) * MaxDuty);

    if (Power > 0.0f)
    {
        WriteDuty(FwdCh, Duty);
        WriteDuty(BwdCh, 0);
    }
    else if (Power < 0.0f)
    {
        WriteDuty(FwdCh, 0);
        WriteDuty(BwdCh, Duty);
    }
    else
    {
        WriteDuty(FwdCh, 0);
        WriteDuty(BwdCh, 0);
    }
}

void Movement::forward(float Power)
{
    setMotor(mot1FwdCh_, mot1BwdCh_, Power);
    setMotor(mot2FwdCh_, mot2BwdCh_, Power);
}

void Movement::backward(float Power)
{
    setMotor(mot1BwdCh_, mot1FwdCh_, Power);
    setMotor(mot2BwdCh_, mot2FwdCh_, Power);
}

void Movement::turnLeft(float Power)
{
    setMotor(mot1FwdCh_, mot1BwdCh_, Power);
    setMotor(mot2BwdCh_, mot2FwdCh_, Power);
}

void Movement::turnRight(float Power)
{
    setMotor(mot1BwdCh_, mot1FwdCh_, Power);
    setMotor(mot2FwdCh_, mot2BwdCh_, Power);
}

void Movement::move(float Linear, float Angular)
{
    constexpr float WheelRadius = 0.058f;
    constexpr float WheelBase = 0.055f;

    float LeftVel =
        (Linear - (WheelBase / 2.0f) * Angular) /
        WheelRadius;

    float RightVel =
        (Linear + (WheelBase / 2.0f) * Angular) /
        WheelRadius;

    float MaxVal =
        std::max(std::fabs(LeftVel),
                 std::fabs(RightVel));

    if (MaxVal > 1.0f)
    {
        LeftVel /= MaxVal;
        RightVel /= MaxVal;
    }

    setMotor(mot1FwdCh_, mot1BwdCh_, LeftVel);
    setMotor(mot2FwdCh_, mot2BwdCh_, RightVel);
}

void Movement::stop()
{
    WriteDuty(mot1FwdCh_, 0);
    WriteDuty(mot1BwdCh_, 0);
    WriteDuty(mot2FwdCh_, 0);
    WriteDuty(mot2BwdCh_, 0);
}