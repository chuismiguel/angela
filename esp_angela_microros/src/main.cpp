#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <movement_handler.h>
#include <joint_handler.h>

// Project libraries
// #include "ota.h"

// Wifi credentials
#define WIFI_SSID "LM2_WIFI"
#define WIFI_PASSWORD "Azirelion15!"

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

// ANGELA COMPONENTS
RosMovementHandler rosMovementHandler;
RosJointHandler rosJointHandler;

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
  rosJointHandler.init(&node, &support, &allocator);
  Serial.println("Setup complete");
}

void loop() {
  delay (50);
  rosMovementHandler.spin();
  rosJointHandler.spin();
  // Handle OTA updates
//   OtaHandler::Handle();
}