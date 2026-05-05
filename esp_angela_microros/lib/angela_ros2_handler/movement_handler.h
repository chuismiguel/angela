#pragma once

// hardware control
#include <movement.h>
#include <pin_definitions.h>

// base class
#include "subscriber.h"

// msg
#include <geometry_msgs/msg/twist.h>

class RosMovementHandler : public BaseSubscriber  {
    public:
        void init(rcl_node_t* node, rclc_support_t* support, rcl_allocator_t* allocator) override;

    private:
        static void subscriptionCallback(const void *msgin, void *context);

        void msgToMovement(const geometry_msgs__msg__Twist *msg_twist);

    private:
        Movement movement_ {angela_pins::MOT1_FWD, angela_pins::MOT1_BWD, angela_pins::MOT2_FWD, angela_pins::MOT2_BWD};
        geometry_msgs__msg__Twist msg_;
};