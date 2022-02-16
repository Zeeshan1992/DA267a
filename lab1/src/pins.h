#ifndef PINS_H
#define PINS_H

#include <stdint.h>
#include <driver/gpio.h>

#define LED_A 13
#define LED_B 14
#define BUTTON_A 26
#define BUTTON_B 27

// init the 4 pins
void initPins(uint8_t led_a, uint8_t led_b, uint8_t button_a, uint8_t button_b);

// switches LED  on if level !=0 or off if level == 0
void setLED(uint8_t pin, uint8_t level);

// tells if button is currently being pressed
uint8_t isButtonPressed(uint8_t button);



#endif