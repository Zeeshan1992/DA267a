#include <sound_gen.h>
#include <sampler.h>
#include <pins.h>
#include <random.h>

/**
 * Author 
 * Abdullahi Farah & Daniel Lone
 * 
 */

void blinkLED(int ms, int times)
{
    for (int i = 0; i < times; i++)
    {
        setLED(led_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(ms));
        setLED(led_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(ms));
    }
}

void waitMs(unsigned int millis)
{
    TickType_t delay = millis / portTICK_PERIOD_MS;
    vTaskDelay(delay);
}

float absval(float a)
{
    if (a < 0)
    {
        return -a;
    }
    else
    {
        return a;
    }
}

void app_main()
{
    initPins(led_pin);

    while (1)
    {
        printf("Starting the game!\n");
        blinkLED(50, 20);

        int randomFreq = getRandomFreq(500, 2000);
        int actualFreq = startSound(randomFreq);
        printf("%d\n", actualFreq);

        waitMs(500);
        stopSound();

        waitMs(500);

        printf("Speak...");
        printf("\n\n\n\n");
        waitMs(1000);
        startSampling(2000);

        waitMs(1000);
        stopSampling();

        float playerFreq = getFrequency();
        printf("player: %.2f, actual: %d\n", playerFreq, actualFreq);
        waitMs(1000);
        if (absval(playerFreq - (float)actualFreq) < 50)
        {
            printf("You win!\n");
            // blink fast for 2 secs
            blinkLED(100, 20);
        }
        else
        {
            printf("You lose!\n");
            // blink slow for 2 secs
            blinkLED(1000, 2);
        }

        printf("Restarting the game \n");
        waitMs(3000);
    }
}