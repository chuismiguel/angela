#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>

#include <movement_handler.h>

#include "ros_error_functions.h"

// Project libraries
// #include "ota.h"

// Wifi credentials
#define WIFI_SSID "LM2_WIFI"
#define WIFI_PASSWORD "Azirelion15!"

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

// ANGELA COMPONENTS
RosMovementHandler rosMovementHandler;

void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    ros_error::rcSoftCheck(rcl_publish(&publisher, &msg, NULL));
    msg.data++;
    Serial.println("Publishing: " + String(msg.data));
  }
}

void setup() {
  // Configure serial transport
  Serial.begin(115200);
  set_microros_wifi_transports(WIFI_SSID, WIFI_PASSWORD, IPAddress(192, 168, 68, 54), 8888);
  delay(2000);

  // Setup ota
//   OtaHandler::Begin(WIFI_SSID, WIFI_PASSWORD);

  allocator = rcl_get_default_allocator();

  //create init_options
  ros_error::rcCheck(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  ros_error::rcCheck(rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));

  rosMovementHandler.init(&node, &support, &allocator);

  // create publisher
  ros_error::rcCheck(rclc_publisher_init_default(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "chatter"));

  // create timer,
  const unsigned int timer_timeout = 1000;
  ros_error::rcCheck(rclc_timer_init_default(
    &timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    timer_callback));

    Serial.println("timer");

  // create executor
  ros_error::rcCheck(rclc_executor_init(&executor, &support.context, 1, &allocator));
  ros_error::rcCheck(rclc_executor_add_timer(&executor, &timer));

Serial.println("executor");

  msg.data = 0;
}

void loop() {
  delay (50);
  ros_error::rcSoftCheck(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
  rosMovementHandler.spin();
  // Handle OTA updates
//   OtaHandler::Handle();
}