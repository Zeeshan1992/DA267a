#include "random.h"
/**
 * Generates random int between min and max
 */
int getRandomFreq(int min,int max){

//float r = (float) esp_random()/UINT32_MAX;

//return (int) (min + r*(max - min));
int rand = (esp_random() % (max + 1 - min) + min);
return rand;
}