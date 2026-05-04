#pragma once 

#include <ESP32Servo.h>  // or <ESP32Servo.h> on ESP32

class JointControl
{
public:
    JointControl(int headPin, int armLeftPin, int armRightPin);

    void begin();

    void setHead(float level);
    void setArms(float level);
    void setAll(float headLevel, float armsLevel);

private:
    float clamp(float value, float min, float max);
    int levelToAngle(float level);

    int headPin_;
    int armLeftPin_;
    int armRightPin_;

    Servo headServo_;
    Servo armLeftServo_;
    Servo armRightServo_;
};