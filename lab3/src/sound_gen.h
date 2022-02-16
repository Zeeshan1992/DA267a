#ifndef SOUND_GEN_H
#define SOUND_GEN_H

#include "soc/sens_reg.h"
#include <driver/dac.h>

/*
Starts generating a sound wave with a certain frequency
the sound will be generated on a given DAC channel (either pin 25 or 26).
Parameter freq: the desired frequency to be set.
Returns the actual frequency that was set.
*/
int startSound(int freq);

/*
Stops the playing sound
*/
void stopSound();

#endif