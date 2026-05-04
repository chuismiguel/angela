
#include "movement.h"

#include <Arduino.h>

// PWM configuration
static constexpr int PWMFreq = 1000;
static constexpr int PWMResolution = 8; // 0–255
static constexpr int MaxDuty = 255;


Movement::Movement(int mot1Fwd, int mot1Bwd, int mot2Fwd, int mot2Bwd)
{
    mot1FwdPin_ = mot1Fwd;
    mot1BwdPin_ = mot1Bwd;
    mot2FwdPin_ = mot2Fwd;
    mot2BwdPin_ = mot2Bwd;

    mot1FwdCh_ = 0;
    mot1BwdCh_ = 1;
    mot2FwdCh_ = 2;
    mot2BwdCh_ = 3;
}

void Movement::begin()
{
    ledcSetup(mot1FwdCh_, PWMFreq, PWMResolution);
    ledcSetup(mot1BwdCh_, PWMFreq, PWMResolution);
    ledcSetup(mot2FwdCh_, PWMFreq, PWMResolution);
    ledcSetup(mot2BwdCh_, PWMFreq, PWMResolution);

    ledcAttachPin(mot1FwdPin_, mot1FwdCh_);
    ledcAttachPin(mot1BwdPin_, mot1BwdCh_);
    ledcAttachPin(mot2FwdPin_, mot2FwdCh_);
    ledcAttachPin(mot2BwdPin_, mot2BwdCh_);

    stop();
}

void Movement::setMotor(int fwdCh, int bwdCh, float power)
{
    // Clamp input to [-1, 1]
    power = constrain(power, -1.0f, 1.0f);

    // Convert to PWM duty (0–255 for 8-bit resolution)
    int duty = (int)(abs(power) * MaxDuty);

    if (power > 0.0f)
    {
        // Forward
        ledcWrite(fwdCh, duty);
        ledcWrite(bwdCh, 0);
    }
    else if (power < 0.0f)
    {
        // Backward
        ledcWrite(fwdCh, 0);
        ledcWrite(bwdCh, duty);
    }
    else
    {
        // Stop
        ledcWrite(fwdCh, 0);
        ledcWrite(bwdCh, 0);
    }
}

void Movement::forward(float power)
{
    setMotor(mot1FwdCh_, mot1BwdCh_, power);
    setMotor(mot2FwdCh_, mot2BwdCh_, power);
}

void Movement::backward(float power)
{
    setMotor(mot1BwdCh_, mot1FwdCh_, power);
    setMotor(mot2BwdCh_, mot2FwdCh_, power);
}

void Movement::turnLeft(float power)
{
    setMotor(mot1FwdCh_, mot1BwdCh_, power);
    setMotor(mot2BwdCh_, mot2FwdCh_ , power);
}

void Movement::turnRight(float power)
{
    setMotor(mot1BwdCh_, mot1FwdCh_, power);
    setMotor(mot2FwdCh_, mot2BwdCh_, power);
}

void Movement::move(float linear, float angular)
{
    // Robot parameters (tweaked to my case)
    static constexpr float WheelRadius = 0.058f;   // meters (example: 3cm)
    static constexpr float WheelBase   = 0.055f;   // meters (distance between wheels)

    // Inverse kinematics (from roboticsbook)
    float leftVel  = (linear - (WheelBase / 2.0f) * angular) / WheelRadius;
    float rightVel = (linear + (WheelBase / 2.0f) * angular) / WheelRadius;

    // Normalize to [-1, 1] (since PWM is limited)
    float maxVal = max(abs(leftVel), abs(rightVel));
    if (maxVal > 1.0f)
    {
        leftVel /= maxVal;
        rightVel /= maxVal;
    }

    // Send to motors
    setMotor(mot1FwdCh_, mot1BwdCh_, leftVel);
    setMotor(mot2FwdCh_, mot2BwdCh_, rightVel);
}

void Movement::stop()
{
    ledcWrite(mot1FwdCh_, 0);
    ledcWrite(mot1BwdCh_, 0);
    ledcWrite(mot2FwdCh_, 0);
    ledcWrite(mot2BwdCh_, 0);
}