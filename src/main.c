
#include <stdio.h>
#include "sdkconfig.h"
#include "mpu_5060_driver.h"
#include <string.h>


#define TAG "MPU-5060 example"


SemaphoreHandle_t print_mux = NULL;

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init()
{
    ESP_LOGE(TAG, "Initializing I2C master mode...");
    i2c_config_t conf;
	memset(&conf, 0, sizeof(i2c_config_t));
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = GPIO_NUM_21;
	conf.scl_io_num = GPIO_NUM_22;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = 100000;
	i2c_param_config(CONFIG_I2C_MASTER_PORT_NUM, &conf);

	ESP_LOGE(TAG, "Initializing I2C master mode complete");

	return i2c_driver_install(CONFIG_I2C_MASTER_PORT_NUM, I2C_MODE_MASTER, 0, 0, 0);
}

void task_mpu_5060(void *ignore) {
    initialize();
    vTaskDelete(NULL);
}

void app_main()
{
    print_mux = xSemaphoreCreateMutex();

    checkErr("Init I2C master", i2c_master_init());
    vTaskDelay(1000 / portTICK_PERIOD_MS);
	xTaskCreate(&task_mpu_5060, "task_mpu_5060", 81920, NULL, 5, NULL);
}