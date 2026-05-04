#pragma once

// std
#include <stdint.h>

class Movement
{
public:
    Movement(int mot1Fwd, int mot1Bwd, int mot2Fwd, int mot2Bwd);

    void begin();

    void forward(float power = 1.0f);
    void backward(float power = 1.0f);
    void turnLeft(float power = 1.0f);
    void turnRight(float power = 1.0f);
    void move(float linear, float angular);
    void stop();

private:
    void setMotor(int fwdCh, int bwdCh, float power);

private:
    int mot1FwdPin_;
    int mot1BwdPin_;
    int mot2FwdPin_;
    int mot2BwdPin_;

    // PWM channels (LEDC)
    uint8_t mot1FwdCh_;
    uint8_t mot1BwdCh_;
    uint8_t mot2FwdCh_;
    uint8_t mot2BwdCh_;
};
