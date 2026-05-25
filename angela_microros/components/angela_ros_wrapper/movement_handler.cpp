// Angela Microros wrapper
#include "movement_handler.h"

// ros errors
#include "ros_error_functions.h"

// logi
#include "esp_log.h"

// math
#include <cmath>

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
        ESP_LOGI("movement_handler", "cmd_vel: linear.x=%f angular.z=%f", msg_twist->linear.x, msg_twist->angular.z);
        self->msgToMovement(msg_twist);
    }
}

void RosMovementHandler::msgToMovement(const geometry_msgs__msg__Twist *msg_twist)
{
    float linear = clamp(msg_twist->linear.x, -1.0f, 1.0f);
    float angular = clamp(msg_twist->angular.z, -3.0f, 3.0f);

    float absLinear =  std::fabs(linear);
    float absAngular = std::fabs(angular);

    const float DeadZone = 0.05f;

    if (absLinear < DeadZone && absAngular < DeadZone)
    {
        ESP_LOGI("movement_handler", "Within deadzone, stopping movement.");
        movement_.stop();
        return;
    }

    movement_.move(linear, angular);
}

void RosMovementHandler::init(rcl_node_t* node, rclc_support_t* support, rcl_allocator_t* allocator, rclc_executor_t* executor)
{
    // Initialize subscriber
    ros_error::rcCheck(rclc_subscription_init_default(
        &subscriber_,
        node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
        CONFIG_MOVEMENT_TOPIC_NAME
    ));

    // Initialize executor for subscriber
    ros_error::rcCheck(rclc_executor_add_subscription_with_context(executor, &subscriber_, &msg_, &RosMovementHandler::subscriptionCallback, this, ON_NEW_DATA));

    // begin movement
    movement_.begin();
}
