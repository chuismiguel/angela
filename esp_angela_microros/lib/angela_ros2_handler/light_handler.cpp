// Angela Microros wrapper
#include "light_handler.h"

// ros errors
#include "ros_error_functions.h"

// arduino
#include <Arduino.h>

void RosLightHandler::subscriptionCallback(const void *msgin, void *context)
{
 const auto *msg_color =
        (const std_msgs__msg__ColorRGBA *)msgin;

    // convert ONCE, hide void* from your logic
    RosLightHandler* self = static_cast<RosLightHandler*>(context);

    if (self != nullptr)
    {
        self->msgToLight(msg_color);
    }
}

void RosLightHandler::msgToLight(const std_msgs__msg__ColorRGBA *msg_color)
{
    Serial.println(
        "Received ColorRGBA: r=" + String(msg_color->r) +
        " g=" + String(msg_color->g) +
        " b=" + String(msg_color->b) +
        " a=" + String(msg_color->a)
    );

    globalLightInstance.setLight(msg_color->r, msg_color->g, msg_color->b);
}

void RosLightHandler::init(rcl_node_t *node, rclc_support_t *support, rcl_allocator_t *allocator)
{
    // Initialize subscriber
    ros_error::rcCheck(rclc_subscription_init_default(
        &subscriber_,
        node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, ColorRGBA),
        "/angela/light"
    ));

    // Initialize executor for subscriber
    ros_error::rcCheck(rclc_executor_init(&executor_, &support->context, 1, allocator));
    ros_error::rcCheck(rclc_executor_add_subscription_with_context(&executor_, &subscriber_, &msg_, &RosLightHandler::subscriptionCallback, this, ON_NEW_DATA));
}
