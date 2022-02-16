#include "i2c_drivers.h"

/**
 * Author
 * Daniel Lone DL
 * Abdullahi Farah AF
 */

void initI2C(int sdapin, int sclpin) //AF
{
    i2c_config_t config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sdapin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = sclpin,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 1000000
    };
    esp_err_t res = i2c_param_config(I2C_NUM_0, &config);
    ESP_ERROR_CHECK(res);

    res = i2c_driver_install(I2C_NUM_0, config.mode, 0, 0, 0);
    ESP_ERROR_CHECK(res);
}

void writeI2C(uint8_t address, uint8_t reg, uint8_t data) //DL
{
    esp_err_t res;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    res = i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, 1);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, reg, 1);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, data, 1);
    ESP_ERROR_CHECK(res);
    i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);
}

void readI2C(uint8_t address, uint8_t reg, uint8_t *buffer, int len) //DL
{
    esp_err_t res;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    res = i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, 1);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, reg, 1);
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);

    // wait a little
    vTaskDelay(10/ portTICK_RATE_MS);


    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_READ, 1);
    ESP_ERROR_CHECK(res);
    res = i2c_master_read(cmd, buffer, len, I2C_MASTER_NACK);
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);

}
