#pragma once

#include <rclc/rclc.h>
#include <rclc/executor.h>

// ros errors
#include "ros_error_functions.h"

class BaseSubscriber {
    public:
        virtual void init(rcl_node_t* node, rclc_support_t* support, rcl_allocator_t* allocator) = 0;
        virtual void spin() {
            ros_error::rcCheck(rclc_executor_spin_some(&executor_, RCL_MS_TO_NS(100)));
        } 

    protected:
        // topic sub
        rcl_subscription_t subscriber_;
        rclc_executor_t executor_;
};