#include <esp_task_wdt.h>
#include <pins.h> // pins defined here
#include <random.h>

/*
    waits for "millis" milliseconds without upsetting the watchdog timer
*/
void waitMs(unsigned int millis)
{
    TickType_t delay = millis / portTICK_PERIOD_MS;
    vTaskDelay(delay);
}
void app_main()
{
    initPins(LED_A, LED_B, BUTTON_A, BUTTON_B);

    while (1)
    {
        // signal that the game is about to start
        // you can flash LEDs in a certain way for it
        setLED(LED_A, 1);
        setLED(LED_B, 1);
        waitMs(500);

        // switch both LEDs off
        setLED(LED_A, 0);
        setLED(LED_B, 0);

        // get a random duration between 3 and 5 seconds
        int random = getRandomsecs(3, 5);

        // wait that random duration
        printf("Game Starting in... %d \n" , random);
        waitMs(random *1000);

        // switch both LEDs ON
        setLED(LED_A, 1);
        setLED(LED_B, 1);

        uint8_t winner = 0;
        while (!winner)
        {
            // check if either button A or B are pressed
            // if any is pressed, set winner to 1 for A or 2 for B
            if (isButtonPressed(BUTTON_A))
            {
                winner = 1;
            }
            else if (isButtonPressed(BUTTON_B))
            {
                winner = 2;
            }
        }

        // A is the winner
        if (winner == 1)
        {
            setLED(LED_A, 1);
            setLED(LED_B, 0);
        }
        // B is the winner
        else if (winner == 2)
        {
            setLED(LED_A, 0);
            setLED(LED_B, 1);
        }
        // let the victor enjoy himself for a sec
        waitMs(1000);

        // restart the game
        setLED(LED_A, 0);
        setLED(LED_B, 0);

        printf("Restarting game \n");
        waitMs(5000);
    }
}