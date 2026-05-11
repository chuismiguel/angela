#pragma once

#include <rcl/rcl.h>

// Light management
#include <lighting.h>
#include <pin_definitions.h>

extern Lights globalLightInstance;

namespace ros_error
{

void rcCheck(rcl_ret_t Rc);
void rcSoftCheck(rcl_ret_t Rc);
}