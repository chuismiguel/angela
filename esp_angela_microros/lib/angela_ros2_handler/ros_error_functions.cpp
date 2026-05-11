#include "ros_error_functions.h"

// Define the global light instance
Lights globalLightInstance(angela_pins::LIGHTS, angela_pins::LIGHT_N);

namespace ros_error
{

void rcCheck(rcl_ret_t Rc)
{
    if (Rc != RCL_RET_OK)
    {
       globalLightInstance.showErrorLoop();
    }
}

void rcSoftCheck(rcl_ret_t Rc)
{
    if (Rc != RCL_RET_OK)
    {
        globalLightInstance.warnLight();
    }
}
}