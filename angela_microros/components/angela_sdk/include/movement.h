#pragma once

#include "driver/gpio.h"
#include "driver/ledc.h"

class Movement
{
public:
    Movement(int Mot1Fwd,
             int Mot1Bwd,
             int Mot2Fwd,
             int Mot2Bwd);

    void begin();

    void forward(float Power);
    void backward(float Power);

    void turnLeft(float Power);
    void turnRight(float Power);

    void move(float Linear, float Angular);

    void stop();

private:
    void setMotor(ledc_channel_t FwdCh,
                  ledc_channel_t BwdCh,
                  float Power);

    void ConfigureChannel(gpio_num_t Pin,
                          ledc_channel_t Channel);

    void WriteDuty(ledc_channel_t Channel,
                   uint32_t Duty);

private:
    gpio_num_t mot1FwdPin_;
    gpio_num_t mot1BwdPin_;
    gpio_num_t mot2FwdPin_;
    gpio_num_t mot2BwdPin_;

    ledc_channel_t mot1FwdCh_;
    ledc_channel_t mot1BwdCh_;
    ledc_channel_t mot2FwdCh_;
    ledc_channel_t mot2BwdCh_;
};