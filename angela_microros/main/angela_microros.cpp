#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"

#include <uros_network_interfaces.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <std_msgs/msg/int32.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

// angela ros wrapper
#include "ros_error_functions.h"
#include "joint_handler.h"
#include "movement_handler.h"

#define ANGELA_PUB_SUB_COMPONENTS 3

// sdk
#include "ultrasonic.h"

#ifdef CONFIG_MICRO_ROS_ESP_XRCE_DDS_MIDDLEWARE
#include <rmw_microros/rmw_microros.h>
#endif

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg;

// ros handling components
RosJointHandler rosJointHandler;
RosMovementHandler rosMovementHandler;

void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{
	RCLC_UNUSED(last_call_time);
	if (timer != NULL) {
		printf("Publishing: %d\n", (int) msg.data);
		ros_error::rcSoftCheck(rcl_publish(&publisher, &msg, NULL));
		msg.data++;
	}
}

void micro_ros_task(void * arg)
{
	// begin global lights
	globalLightInstance.Begin();

	    ultrasonic_sensor_t sensor = {
        .pin = static_cast<gpio_num_t>(CONFIG_US_SHARED_PIN)
    };

    ultrasonic_init(&sensor);

	rcl_allocator_t allocator = rcl_get_default_allocator();
	rclc_support_t support;

	rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
	ros_error::rcCheck(rcl_init_options_init(&init_options, allocator));

#ifdef CONFIG_MICRO_ROS_ESP_XRCE_DDS_MIDDLEWARE
	rmw_init_options_t* rmw_options = rcl_init_options_get_rmw_init_options(&init_options);

	// Static Agent IP and port can be used instead of autodisvery.
	ros_error::rcCheck(rmw_uros_options_set_udp_address(CONFIG_MICRO_ROS_AGENT_IP, CONFIG_MICRO_ROS_AGENT_PORT, rmw_options));
	//ros_error::rcCheck(rmw_uros_discover_agent(rmw_options));
#endif

	// create init_options
	ros_error::rcCheck(rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator));

	// create node
	rcl_node_t node;
	ros_error::rcCheck(rclc_node_init_default(&node, "esp32_int32_publisher", "", &support));

	// create publisher
	ros_error::rcCheck(rclc_publisher_init_default(
		&publisher,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
		"freertos_int32_publisher"));

	// create timer,
	rcl_timer_t timer;
	const unsigned int timer_timeout = 1000;
	ros_error::rcCheck(rclc_timer_init_default(
		&timer,
		&support,
		RCL_MS_TO_NS(timer_timeout),
		timer_callback));
	
	// create executor
	rclc_executor_t executor;
	ros_error::rcCheck(rclc_executor_init(&executor, &support.context, ANGELA_PUB_SUB_COMPONENTS, &allocator));
	ros_error::rcCheck(rclc_executor_add_timer(&executor, &timer));

	msg.data = 0;

	rosJointHandler.init(&node, &support, &allocator, &executor);
	rosMovementHandler.init(&node, &support, &allocator, &executor);

	globalLightInstance.ShowSuccess();

	while(1){
		rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
		usleep(10000);
	}

	// free resources
	ros_error::rcCheck(rcl_publisher_fini(&publisher, &node));
	ros_error::rcCheck(rcl_node_fini(&node));

  	vTaskDelete(NULL);
}

extern "C" void app_main(void)
{
#if defined(CONFIG_MICRO_ROS_ESP_NETIF_WLAN) || defined(CONFIG_MICRO_ROS_ESP_NETIF_ENET)
    ESP_ERROR_CHECK(uros_network_interface_initialize());
#endif

    //pin micro-ros task in APP_CPU to make PRO_CPU to deal with wifi:
    xTaskCreate(micro_ros_task,
            "uros_task",
            CONFIG_MICRO_ROS_APP_STACK,
            NULL,
            CONFIG_MICRO_ROS_APP_TASK_PRIO,
            NULL);
}
