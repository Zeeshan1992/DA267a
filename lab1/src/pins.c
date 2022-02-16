#include <pins.h>



// init the 4 pins
void initPins(uint8_t led_a, uint8_t led_b, uint8_t button_a, uint8_t button_b)
{
    gpio_config_t config;
    config.pin_bit_mask = (uint64_t)1 << led_a | (uint64_t)1 << led_b; // | or & ?? should be & but all examples give me  |
    config.mode = GPIO_MODE_OUTPUT;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    esp_err_t res = gpio_config(&config);
    ESP_ERROR_CHECK(res);

    config.pin_bit_mask = (u_int64_t)1 << button_a | (uint64_t)1 << button_b;
    config.mode = GPIO_MODE_INPUT;
    config.pull_up_en = GPIO_PULLUP_ENABLE;
    res = gpio_config(&config);
    ESP_ERROR_CHECK(res);
}


// switches LED  on if level !=0 or off if level == 0
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


// tells if button is currently being pressed
uint8_t isButtonPressed(uint8_t button)
{
    uint8_t err;
    if (gpio_get_level(button) == 0)
    {
        err = 1;
    }
    else
    {
        err = 0;
    }
    return err;  
}
