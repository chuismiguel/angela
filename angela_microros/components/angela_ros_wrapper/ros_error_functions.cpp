#include "ros_error_functions.h"

// log
#include "esp_log.h"

static const char *TAG = "ROS_ERROR";

// Define the global light instance
Lights globalLightInstance(static_cast<gpio_num_t>(CONFIG_LIGHT_PIN), static_cast<uint32_t>(CONFIG_LIGHT_PIXEL_COUNT));

namespace ros_error
{

void rcCheck(rcl_ret_t Rc)
{
    if (Rc != RCL_RET_OK)
    {
        ESP_LOGE(TAG, "Fatal ROS error: rcl_ret_t = %ld", Rc);

       globalLightInstance.ShowErrorLoop();
    }
}

void rcSoftCheck(rcl_ret_t Rc)
{
    if (Rc != RCL_RET_OK)
    {
        ESP_LOGW(TAG, "Non-fatal ROS warning: rcl_ret_t = %ld", Rc);

        globalLightInstance.WarnLight();
    }
}
}