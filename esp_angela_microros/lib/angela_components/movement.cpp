
#include "movement.h"

#include <Arduino.h>


Movement::Movement(int mot1Fwd, int mot1Bwd, int mot2Fwd, int mot2Bwd)
{
    mot1FwdPin_ = mot1Fwd;
    mot1BwdPin_ = mot1Bwd;
    mot2FwdPin_ = mot2Fwd;
    mot2BwdPin_ = mot2Bwd;
}

void Movement::begin()
{
    pinMode(mot1FwdPin_, OUTPUT);
    pinMode(mot1BwdPin_, OUTPUT);
    pinMode(mot2FwdPin_, OUTPUT);
    pinMode(mot2BwdPin_, OUTPUT);

    stop();
}

void Movement::forward()
{
    digitalWrite(mot1FwdPin_, HIGH);
    digitalWrite(mot1BwdPin_, LOW);
    digitalWrite(mot2FwdPin_, HIGH);
    digitalWrite(mot2BwdPin_, LOW);
}

void Movement::backward()
{
    digitalWrite(mot1FwdPin_, LOW);
    digitalWrite(mot1BwdPin_, HIGH);
    digitalWrite(mot2FwdPin_, LOW);
    digitalWrite(mot2BwdPin_, HIGH);
}

void Movement::turnLeft()
{
    digitalWrite(mot1FwdPin_, LOW);
    digitalWrite(mot1BwdPin_, HIGH);
    digitalWrite(mot2FwdPin_, HIGH);
    digitalWrite(mot2BwdPin_, LOW);
}

void Movement::turnRight()
{
    digitalWrite(mot1FwdPin_, HIGH);
    digitalWrite(mot1BwdPin_, LOW);
    digitalWrite(mot2FwdPin_, LOW);
    digitalWrite(mot2BwdPin_, HIGH);
}

void Movement::stop()
{
    digitalWrite(mot1FwdPin_, LOW);
    digitalWrite(mot1BwdPin_, LOW);
    digitalWrite(mot2FwdPin_, LOW);
    digitalWrite(mot2BwdPin_, LOW);
}