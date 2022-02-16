#include <pins.h>

/**
 * Author 
 * Abdullahi Farah & Daniel Lone
 * 
 */

void initPins(uint8_t led)
{
    gpio_config_t config;
    config.pin_bit_mask = (uint64_t)1 << led;
    config.mode = GPIO_MODE_OUTPUT;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    esp_err_t res = gpio_config(&config);
    ESP_ERROR_CHECK(res);
}

void setLED(uint8_t pin, uint8_t level)
{
    if (level)
    {
        gpio_set_level(pin, 1);
    }
    else
    {
        gpio_set_level(pin, 0);
    }   
}
