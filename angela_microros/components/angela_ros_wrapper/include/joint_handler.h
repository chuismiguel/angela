#pragma once

// hardware control
#include "iot_servo.h"

// base class
#include "subscriber.h"

// msg
#include <sensor_msgs/msg/joint_state.h>

class RosJointHandler : public BaseSubscriber  {
    public:
        void init(rcl_node_t* node, rclc_support_t* support, rcl_allocator_t* allocator, rclc_executor_t* executor) override;

    private:
        static void subscriptionCallback(const void *msgin, void *context);

        void msgToMovement(const sensor_msgs__msg__JointState *msg_joint_state);

        void writeJointValues(float armPosition, float headPosition);

    private:
        servo_config_t servoCfg_ {};
        sensor_msgs__msg__JointState msg_;
};