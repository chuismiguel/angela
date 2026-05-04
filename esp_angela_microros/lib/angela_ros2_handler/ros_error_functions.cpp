#include "ros_error_functions.h"

#include <Arduino.h>

namespace ros_error
{

void ErrorLoop()
{
    while (true)
    {
        // TODO: Add US Led Blinking
        Serial.println("Entered error state");
        delay(1000);
    }
}

void rcCheck(rcl_ret_t Rc)
{
    if (Rc != RCL_RET_OK)
    {
        ErrorLoop();
    }
}

void rcSoftCheck(rcl_ret_t Rc)
{
    if (Rc != RCL_RET_OK)
    {
        // Do nothing (soft fail)
    }
}
}