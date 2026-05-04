#pragma once

class Movement
{
public:
    Movement(int mot1Fwd, int mot1Bwd, int mot2Fwd, int mot2Bwd);

    void begin();

    void forward();
    void backward();
    void turnLeft();
    void turnRight();
    void stop();

private:
    int mot1FwdPin_;
    int mot1BwdPin_;
    int mot2FwdPin_;
    int mot2BwdPin_;
};
