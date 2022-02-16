#ifndef PINS_H
#define PINS_H

#include <stdint.h>
#include <driver/gpio.h>

#define led_pin 13

// init the 4 pins
void initPins(uint8_t pin);

// switches LED  on if level !=0 or off if level == 0
void setLED(uint8_t pin, uint8_t level);

#endif