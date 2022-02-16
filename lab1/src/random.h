#ifndef RANDOM_H
#define RANDOM_H

#include <esp_system.h>

/* Returns a random number between min and max
min minimum number of ms
max  maximum number of ms
*/
int getRandomsecs(int min, int max);

#endif