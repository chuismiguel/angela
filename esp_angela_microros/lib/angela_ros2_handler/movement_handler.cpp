// Angela Microros wrapper
#include "movement_handler.h"

// ros errors
#include "ros_error_functions.h"

// arduino
#include <Arduino.h>


float clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void RosMovementHandler::subscriptionCallback(const void *msgin, void *context)
{

    const auto *msg_twist =
        (const geometry_msgs__msg__Twist *)msgin;

    // convert ONCE, hide void* from your logic
    RosMovementHandler* self = static_cast<RosMovementHandler*>(context);


    if (self != nullptr)
    {
        Serial.println(
            "cmd_vel: linear.x=" + String(msg_twist->linear.x) +
            " angular.z=" + String(msg_twist->angular.z)
        );
        self->msgToMovement(msg_twist);
    }
}

void RosMovementHandler::msgToMovement(const geometry_msgs__msg__Twist *msg_twist)
{
    float linear = clamp(msg_twist->linear.x, -1.0f, 1.0f);
    float angular = clamp(msg_twist->angular.z, -3.0f, 3.0f);

    float absLinear = fabs(linear);
    float absAngular = fabs(angular);

    const float DeadZone = 0.05f;

    if (absLinear < DeadZone && absAngular < DeadZone)
    {
        Serial.println(
            "Stopping"
        );
        movement_.stop();
        return;
    }

    movement_.move(linear, angular);
}

void RosMovementHandler::init(rcl_node_t* node, rclc_support_t* support, rcl_allocator_t* allocator)
{
    // Initialize subscriber
    ros_error::rcCheck(rclc_subscription_init_default(
        &subscriber_,
        node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
        "/angela/cmd_vel"
    ));

    // Initialize executor for subscriber
    ros_error::rcCheck(rclc_executor_init(&executor_, &support->context, 1, allocator));
    ros_error::rcCheck(rclc_executor_add_subscription_with_context(&executor_, &subscriber_, &msg_, &RosMovementHandler::subscriptionCallback, this, ON_NEW_DATA));

    // begin movement
    movement_.begin();
}
