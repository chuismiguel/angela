#pragma once

// hardware control
#include <lighting.h>

// base class
#include "subscriber.h"

// msg
#include <std_msgs/msg/color_rgba.h>

class RosLightHandler : public BaseSubscriber  {

    public:
        void init(rcl_node_t* node, rclc_support_t* support, rcl_allocator_t* allocator) override;
    private:
        static void subscriptionCallback(const void *msgin, void *context);
        void msgToLight(const std_msgs__msg__ColorRGBA *msg_color);
    private:
        std_msgs__msg__ColorRGBA msg_;
};