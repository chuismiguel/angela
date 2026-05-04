#include <movement.h>

#include <pin_definitions.h>

#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <geometry_msgs/msg/twist.h>

class RosMovementHandler {
    public:
        void init(rcl_node_t* node, rclc_support_t* support, rcl_allocator_t* allocator);

        void spin();
    private:
        Movement movement_ {angela_pins::MOT1_FWD, angela_pins::MOT1_BWD, angela_pins::MOT2_FWD, angela_pins::MOT2_BWD};

        // topic sub
        rcl_subscription_t subscriber_;
        rclc_executor_t executor_;
        geometry_msgs__msg__Twist msg_;

};