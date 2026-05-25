#pragma once

// hardware control
#include "movement.h"

// base class
#include "subscriber.h"

// msg
#include <geometry_msgs/msg/twist.h>

class RosMovementHandler : public BaseSubscriber  {
    public:
        void init(rcl_node_t* node, rclc_support_t* support, rcl_allocator_t* allocator, rclc_executor_t* executor) override;

    private:
        static void subscriptionCallback(const void *msgin, void *context);

        void msgToMovement(const geometry_msgs__msg__Twist *msg_twist);

    private:
        Movement movement_ {CONFIG_MOT1_FWD_PIN, CONFIG_MOT1_BWD_PIN, CONFIG_MOT2_FWD_PIN, CONFIG_MOT2_BWD_PIN};
        geometry_msgs__msg__Twist msg_;
};