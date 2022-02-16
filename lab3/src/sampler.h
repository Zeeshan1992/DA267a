#ifndef SAMPLER_H
#define SAMPLER_H

#include <driver/adc.h>
#include <driver/timer.h>
#include <esp_task_wdt.h>

/**
 * Starts sampling on a given pin with a given fequency.
 * Parameter freq: the sampling frequency
 */
void startSampling(int frequency);

/**
 * Stops the sampling process.
 */
void stopSampling();

/**
 * counts the frequency by taking the number of crossing and dividing it by 2 * time,
 * it samples for approximitely 1 sec -> crossing / 2
 */
float getFrequency();

#endif