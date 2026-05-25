#include "joint_handler.h"

// ros errors
#include "ros_error_functions.h"

// msg handling
#include <micro_ros_utilities/type_utilities.h>

// log
#include "esp_log.h"

void RosJointHandler::subscriptionCallback(const void *msgin, void *context)
{
     ESP_LOGI("joint_handler", " Received message in callback\n");
     const auto *msg_joint_state =
        (const sensor_msgs__msg__JointState *)msgin;

    // convert ONCE, hide void* from your logic
    RosJointHandler* self = static_cast<RosJointHandler*>(context);

    if (self != nullptr)
    {
        self->msgToMovement(msg_joint_state);
    }
}

void RosJointHandler::writeJointValues(float armPosition, float headPosition)
{
    ESP_LOGI("joint_handler",
             " Writing arm: %.4f, head: %.4f",
             armPosition, headPosition);

    iot_servo_write_angle(LEDC_LOW_SPEED_MODE, servoCfg_.channels.ch[0], armPosition);
    iot_servo_write_angle(LEDC_LOW_SPEED_MODE, servoCfg_.channels.ch[1],  180.0f - armPosition);
    iot_servo_write_angle(LEDC_LOW_SPEED_MODE, servoCfg_.channels.ch[2], headPosition);
}

void RosJointHandler::msgToMovement(const sensor_msgs__msg__JointState *msg_joint_state)
{

    ESP_LOGI("joint_handler", "Received JointState:");

    if (msg_joint_state->name.size != 2 ||
        msg_joint_state->position.size != 2)
    {
        ESP_LOGW("joint_handler",
                "Invalid JointState size. name=%zu position=%zu",
                msg_joint_state->name.size,
                msg_joint_state->position.size);
        return;
    }

    double ArmPosition = 0.0;
    double HeadPosition = 0.0;

    bool ArmFound = false;
    bool HeadFound = false;

    for (size_t i = 0; i < msg_joint_state->name.size; i++)
    {
        const char* JointName = msg_joint_state->name.data[i].data;

        double Position = msg_joint_state->position.data[i];

        ESP_LOGI("joint_handler",
                "  Joint: %s | Position: %.4f",
                JointName, Position);

        if (strcmp(JointName, CONFIG_ARM_JOINT_NAME) == 0)
        {
            ArmPosition = Position;
            ArmFound = true;
        }
        else if (strcmp(JointName, CONFIG_HEAD_JOINT_NAME) == 0)
        {
            HeadPosition = Position;
            HeadFound = true;
        }
    }

    if (ArmFound && HeadFound)
    {
        writeJointValues(ArmPosition, HeadPosition);
    }
}

void RosJointHandler::init(rcl_node_t *node, rclc_support_t *support, rcl_allocator_t *allocator, rclc_executor_t *executor)
{    
   static micro_ros_utilities_memory_conf_t conf = {0};

    bool success = micro_ros_utilities_create_message_memory(
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, JointState),
    &msg_,
    conf
    );

    // Initialize subscriber
    ros_error::rcCheck(rclc_subscription_init_default(
        &subscriber_,
        node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, JointState),
        CONFIG_JOINT_STATE_TOPIC_NAME
    ));

    // Initialize executor for subscriber
    ros_error::rcCheck(rclc_executor_add_subscription_with_context(executor, &subscriber_, &msg_, &RosJointHandler::subscriptionCallback, this, ON_NEW_DATA));

    // configure servos
    servoCfg_.max_angle  = 180;
    servoCfg_.min_width_us  = 500;
    servoCfg_.max_width_us  = 2500;
    servoCfg_.freq = 50;
    servoCfg_.timer_number  = LEDC_TIMER_0;
    servoCfg_.channels.servo_pin[0] = static_cast<gpio_num_t>(CONFIG_LEFT_ARM_SERVO_PIN);
    servoCfg_.channels.servo_pin[1] = static_cast<gpio_num_t>(CONFIG_RIGHT_ARM_SERVO_PIN);
    servoCfg_.channels.servo_pin[2] = static_cast<gpio_num_t>(CONFIG_HEAD_SERVO_PIN);
    servoCfg_.channels.ch[0] = LEDC_CHANNEL_0;
    servoCfg_.channels.ch[1] = LEDC_CHANNEL_1;
    servoCfg_.channels.ch[2] = LEDC_CHANNEL_2;
    servoCfg_.channel_number  = 3;

    iot_servo_init(LEDC_LOW_SPEED_MODE, &servoCfg_);
}
