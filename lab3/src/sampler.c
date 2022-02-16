#include <sampler.h>

/**
 * Author 
 * Abdullahi Farah & Daniel Lone
 * 
 */

int average = 1387;
volatile int prevSample;
volatile int sample = 1387;
int crossing = 0;

/**
 * We have an average value that the adc prints in silence, so we detect the crossing by saving a previous sample and comparing our newly takien sample with it
 * so if our newly taken sample is below the average value and the prevous sample is higher than the average that means a crossing (if both sample and prev sample would be below average it would
 * not mean a crossing) and vice versa
 * worked together with abdullahi on the crossing detection algorithm
 */
void IRAM_ATTR isr_Handler()
{
    uint32_t timer_intr = timer_group_get_intr_status_in_isr(TIMER_GROUP_0);

    if(timer_intr & TIMER_INTR_T0)
    {
        timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0);
        
    }
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0,TIMER_0);  
    prevSample = sample;
    sample = adc1_get_raw(ADC1_GPIO32_CHANNEL);
    if( (sample < average && prevSample > average) || (sample > average && prevSample < average) )  
    {
        crossing++;
    }

}
/*
* We want to sample with a freq of 20 kHz
*/
void startSampling(int frequency)
{
    adc_power_on();
    adc_gpio_init(ADC_UNIT_1, ADC1_GPIO32_CHANNEL);
    adc1_config_width(ADC_WIDTH_BIT_12);

    timer_config_t config = {
        .auto_reload = TIMER_AUTORELOAD_EN, // we want timer to auto reload after innteruppt
        .alarm_en = TIMER_ALARM_EN,  // enable alarm
        .counter_en = TIMER_PAUSE, 
        .counter_dir = TIMER_COUNT_UP,  // counter will count up
        .divider = 2,  // 80 Mhz / 2 * compare register 
        .intr_type = TIMER_INTR_LEVEL 
    };
       timer_init(TIMER_GROUP_0, TIMER_0, &config);
       timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0x00000000ULL); // setting the inital counter value to 0
       timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, frequency);  // compare register is 2000 so output freq is 80MHz / 2 * 2000 = 20 kHz
       timer_enable_intr(TIMER_GROUP_0, TIMER_0);
       timer_isr_register(TIMER_GROUP_0,TIMER_0, isr_Handler, NULL, ESP_INTR_FLAG_IRAM, NULL);

       timer_start(TIMER_GROUP_0, TIMER_0);

}

void stopSampling()
{
    timer_deinit(TIMER_GROUP_0, TIMER_0);
}

float getFrequency() 
{
    float frequency = crossing / 2;  // freq is crossing / (2* time) time is approximately 1 sec
    return frequency;
}