#pragma once

// hardware control
#include <joint_control.h>
#include <pin_definitions.h>

// base class
#include "subscriber.h"

// msg
#include <sensor_msgs/msg/joint_state.h>

class RosJointHandler : public BaseSubscriber  {
    public:
        void init(rcl_node_t* node, rclc_support_t* support, rcl_allocator_t* allocator) override;

    private:
        static void subscriptionCallback(const void *msgin, void *context);

        void msgToMovement(const sensor_msgs__msg__JointState *msg_joint_state);

    private:
        JointControl jointControl_ {angela_pins::HEAD, angela_pins::LEFT_ARM, angela_pins::RIGHT_ARM};
        sensor_msgs__msg__JointState msg_;
};