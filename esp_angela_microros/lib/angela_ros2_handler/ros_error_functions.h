#pragma once

#include <rcl/rcl.h>


namespace ros_error
{
void ErrorLoop();

void rcCheck(rcl_ret_t Rc);
void rcSoftCheck(rcl_ret_t Rc);
}