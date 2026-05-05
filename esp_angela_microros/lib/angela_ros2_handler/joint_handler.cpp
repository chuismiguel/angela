#include "joint_handler.h"

// ros errors
#include "ros_error_functions.h"

// msg handling
#include <micro_ros_utilities/type_utilities.h>

void RosJointHandler::subscriptionCallback(const void *msgin, void *context)
{
     const auto *msg_joint_state =
        (const sensor_msgs__msg__JointState *)msgin;

    // convert ONCE, hide void* from your logic
    RosJointHandler* self = static_cast<RosJointHandler*>(context);

    if (self != nullptr)
    {
        self->msgToMovement(msg_joint_state);
    }
}

void RosJointHandler::msgToMovement(const sensor_msgs__msg__JointState *msg_joint_state)
{
     size_t jointCount = msg_joint_state->name.size;

    printf("Received JointState:\n");

    for (size_t i = 0; i < jointCount; i++)
    {
        const auto& jointName = msg_joint_state->name.data[i];

        double position = 0.0;
        if (i < msg_joint_state->position.size) {
            position = msg_joint_state->position.data[i];
        } else {
            printf("No data in position for joint: %s of size %zu\n", jointName, msg_joint_state->position.size);
        }

        printf("  Joint: %s | Position: %.4f\n", jointName, position);
    }
}

void RosJointHandler::init(rcl_node_t *node, rclc_support_t *support, rcl_allocator_t *allocator)
{
    // alloc message
    static micro_ros_utilities_memory_conf_t conf = {0};
    conf.max_string_capacity = 10;
    conf.max_basic_type_sequence_capacity = 5;
    conf.max_ros2_type_sequence_capacity = 5;

    micro_ros_utilities_create_message_memory(
        ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, JointState),
        &msg_,
        conf
        );

    // Initialize subscriber
    ros_error::rcCheck(rclc_subscription_init_default(
        &subscriber_,
        node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, JointState),
        "/angela/joint_state"
    ));

    // Initialize executor for subscriber
    ros_error::rcCheck(rclc_executor_init(&executor_, &support->context, 1, allocator));
    ros_error::rcCheck(rclc_executor_add_subscription_with_context(&executor_, &subscriber_, &msg_, &RosJointHandler::subscriptionCallback, this, ON_NEW_DATA));

    jointControl_.begin();
}
