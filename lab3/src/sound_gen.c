#include "sound_gen.h"

/**
 * Author 
 * Abdullahi Farah & Daniel Lone
 * 
 */

int startSound(int freq)
{
   REG_SET_BIT(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN);  // Enables the cosinus waveform generator
   REG_SET_BIT(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1); // connects the generator ti dac channel 
   int freq_step = freq/122;
   if (freq_step % 10 > 4)
   {
       freq_step++;
   }
   
   REG_SET_FIELD(SENS_SAR_DAC_CTRL1_REG, SENS_SW_FSTEP, freq_step); // sets the generator's freq
   REG_SET_FIELD(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV1, 2); // by writing 2 (10) we invert msb

   REG_CLR_BIT(SENS_SAR_DAC_CTRL1_REG,SENS_DAC_SCALE1); // set bits to 0
   REG_CLR_BIT(SENS_SAR_DAC_CTRL1_REG,SENS_DAC_DC1); // set bits to 0
   int aFreq = (8000000 * freq_step)/65536;

   dac_output_enable(DAC_CHANNEL_1);  // activates the dac

   return aFreq;
}

void stopSound()
{
    dac_output_disable(DAC_CHANNEL_1);  // deactivates the dac
}