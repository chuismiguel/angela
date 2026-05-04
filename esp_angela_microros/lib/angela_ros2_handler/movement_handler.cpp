// Angela Microros wrapper
#include "movement_handler.h"
#include "ros_error_functions.h"

#include <Arduino.h>

void subscription_callback(const void *msgin)
{
  const geometry_msgs__msg__Twist *msg_twist = (const geometry_msgs__msg__Twist *)msgin;
  
  Serial.println("Received cmd_vel: linear.x=" + String(msg_twist->linear.x) + " angular.z=" + String(msg_twist->angular.z));
}

void RosMovementHandler::init(rcl_node_t* node, rclc_support_t* support, rcl_allocator_t* allocator)
{
    // Initialize subscriber
    ros_error::rcCheck(rclc_subscription_init_default(
        &subscriber_,
        node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
        "cmd_vel"
    ));

    // Initialize executor for subscriber
    ros_error::rcCheck(rclc_executor_init(&executor_, &support->context, 1, allocator));
    ros_error::rcCheck(rclc_executor_add_subscription(&executor_, &subscriber_, &msg_, &subscription_callback, ON_NEW_DATA));
}

void RosMovementHandler::spin()
{
    ros_error::rcCheck(rclc_executor_spin_some(&executor_, RCL_MS_TO_NS(100)));
}