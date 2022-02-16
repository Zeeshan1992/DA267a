#include <random.h>

/* Returns a random number between min and max
min minimum number of ms
max  maximum number of ms
*/
int getRandomsecs(int min, int max)
{
    int rand = (esp_random() % (max + 1 - min) + min);
    return rand;
}