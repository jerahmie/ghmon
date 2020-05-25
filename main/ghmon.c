/* ghmon - Main routine for Green house monitoring routine.
*/

#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "display/ssd1306.h"

static const char *TAG = "ghmon";

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)



/* I2C definitions */
#define DATA_LENGTH 512

/* I2C Master definitions */
#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL /*!< gpio number for i2c master clock */
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA /*!< gpio number for i2c master data */
#define I2C_MASTER_NUM I2C_NUMBER(CONFIG_I2C_MASTER_PORT_NUM)  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ CONFIG_I2C_MASTER_FREQUENCY  /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */

/* I2C Slave definitions */
#define I2C_SLAVE_SCL_IO CONFIG_I2C_SLAVE_SCL
#define I2C_SLAVE_SDA_IO CONFIG_I2C_SLAVE_SDA
#define ESP_SLAVE_ADDR CONFIG_I2C_SLAVE_ADDRESS /*!< ESP32 slave address, any 7bit value */
#define I2C_SLAVE_NUM I2C_NUMBER(CONFIG_I2C_SLAVE_PORT_NUM)  /*!< I2C port number for slave dev */
#define I2C_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH)
#define I2C_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH)

#define SSD1306_ADDR CONFIG_SSD1306_ADDR
#define CCS811_ADDR CONFIG_CCS811_ADDR

SemaphoreHandle_t print_mux = NULL;

/**
 * @brief I2C master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf_master;
    conf_master.mode = I2C_MODE_MASTER;
    conf_master.sda_io_num = I2C_MASTER_SDA_IO;
    conf_master.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_master.scl_io_num = I2C_MASTER_SCL_IO;
    conf_master.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_master.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf_master);
    return i2c_driver_install(i2c_master_port, conf_master.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

/**
 * @brief I2C slave initialization
 */
 static esp_err_t i2c_slave_init(void)
 {
    int i2c_slave_port = I2C_SLAVE_NUM;
    i2c_config_t conf_slave;
    conf_slave.mode = I2C_MODE_SLAVE;
    conf_slave.sda_io_num = I2C_SLAVE_SDA_IO;
    conf_slave.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.scl_io_num = I2C_SLAVE_SCL_IO;
    conf_slave.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf_slave.slave.addr_10bit_en = 0;
    conf_slave.slave.slave_addr = ESP_SLAVE_ADDR;
    i2c_param_config(i2c_slave_port, &conf_slave);
    return i2c_driver_install(i2c_slave_port, conf_slave.mode, I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0);
 }

/**
* @brief print system info
*/
int print_sysinfo(void)
{   
    printf("Hello ESP32!\n");
    printf("SSD1306_ADDR 0x%x\n", SSD1306_ADDR);
    printf("CCS811_ADDR: 0x%x\n", CCS811_ADDR);
    printf("SET_CONTRAST_CONTROL 0x%x\n", SET_CONTRAST_CONTROL);
    fflush(stdout);
    return 0;
}


void app_main(void)
{
    int status;
    print_mux = xSemaphoreCreateMutex();
    ESP_ERROR_CHECK(i2c_slave_init());
    ESP_ERROR_CHECK(i2c_master_init());
    //printf("Return Status: %d\n", status);
}