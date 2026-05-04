#include "joint_control.h"

JointControl::JointControl(int headPin, int armLeftPin, int armRightPin)
{
    headPin_ = headPin;
    armLeftPin_ = armLeftPin;
    armRightPin_ = armRightPin;
}

void JointControl::begin()
{
    headServo_.attach(headPin_);
    armLeftServo_.attach(armLeftPin_);
    armRightServo_.attach(armRightPin_);

    setHead(0.0f);
    setArms(-0.45f);
}

float JointControl::clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int JointControl::levelToAngle(float level)
{
    // Map [-1, 1] → [0, 180]
    float normalized = (level + 1.0f) * 0.5f;
    return (int)(normalized * 180.0f);
}

void JointControl::setHead(float level)
{
    level = clamp(level, -1.0f, 1.0f);

    int angle = levelToAngle(level);
    headServo_.write(angle);
}

void JointControl::setArms(float level)
{
    level = clamp(level, -0.45f, 0.15f);

    int angleLeft = levelToAngle(level);
    int angleRight = levelToAngle(-level);

    armLeftServo_.write(angleLeft);
    armRightServo_.write(angleRight);
}

void JointControl::setAll(float headLevel, float armsLevel)
{
    setHead(headLevel);
    setArms(armsLevel);
}