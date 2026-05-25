#pragma once

#include <rclc/rclc.h>
#include <rclc/executor.h>

// ros errors
#include "ros_error_functions.h"

class BaseSubscriber {
    public:
        virtual void init(rcl_node_t* node, rclc_support_t* support, rcl_allocator_t* allocator, rclc_executor_t* executor) = 0;
    protected:
        // topic sub
        rcl_subscription_t subscriber_;
};