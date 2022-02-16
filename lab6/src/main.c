/*
* author daniel lone 
* abdullahi farah
*/

#include <stdio.h>
#include <stdlib.h>
#include <esp32/rom/ets_sys.h>
#include <esp_task_wdt.h>
#include "driver/gpio.h"


#define LED_PIN_LEVEL_UP 12
#define LED_PIN_LEVEL_MIDDLE 14
#define LED_PIN_LEVEL_DOWN 27
#define BUTTON_PIN 26

#define UPPER 0
#define MIDDLE 1
#define LOWER 2

#define EMPTY -1

#define delay 500

#define PUSH_TIME_US 250000 // 250 ms

// Used to represent a travel need of a passenger.
 struct travel_need {
    int origin;
    int destination;
};


void idle(struct travel_need *travel);
void move(struct travel_need *travel);
void load(struct travel_need *travel);
void unload(struct travel_need *travel);

void elevator_level(int);


// for keeping track on the position
int elevatorPos = 2;

int i = 0;
// Function ptr
void (*functionPtr) (struct travel_need*);

// Used to not allow button pushes that are too close to each other in time
static volatile uint64_t lastPush = -PUSH_TIME_US;

//Just a counter keeping track of which travel need is next to process.
static volatile int travel_need_counter = 0;

// This data structure holds information about 
static volatile struct travel_need travel_needs[50];

struct travel_need test[50];
// This function is called when button is pushed
static void handle_push(void *arg) 
{

    // Disable interrupts
    gpio_intr_disable(BUTTON_PIN);

    // Get the current time 
    uint64_t now = esp_timer_get_time();


    // If enough time passed, we should consider this event as a genuine push
    if ((now - lastPush) > PUSH_TIME_US) {
       
        lastPush = now;

        //Get next travel need from list and do something with it
        struct travel_need current_travel_need = travel_needs[travel_need_counter];

        test[travel_need_counter] = current_travel_need;

        //Increase travel need counter
        travel_need_counter++;


    } // else ignore



    // Re-enable interrupts
    gpio_intr_enable(BUTTON_PIN);
}



void idle(struct travel_need *travel)
{
    printf("idle");
     printf("\n");
    if (travel->origin == elevatorPos)
    {
        elevator_level(elevatorPos);
        functionPtr = load;
        }else
        {
            if (elevatorPos == UPPER)
            {
                while (travel->origin != elevatorPos)  //elevator move to position ++ since upper is 0 and lowest is 2
                {
                    ++elevatorPos;
                    elevator_level(elevatorPos);
                    vTaskDelay(delay);
                }
                functionPtr = load;
            }
            else if (elevatorPos == MIDDLE)
            {
                elevatorPos = travel->origin;
                elevator_level(elevatorPos);
                vTaskDelay(delay);
                functionPtr = load;
            }
            else if (elevatorPos == LOWER)
            {
                while (travel->origin != elevatorPos) // elevator move to position -- since low is 2 and upper is 0
                {
                    --elevatorPos;
                    elevator_level(elevatorPos);
                    vTaskDelay(delay);
                }
                functionPtr = load;
            }
    }    
}

void move(struct travel_need *travel) 
{   
    printf("move");
     printf("\n");
    if (travel->destination == elevatorPos)  
    {
        elevator_level(elevatorPos);
        functionPtr = unload;
    }else
    {
        if (elevatorPos == UPPER)
        {
            while (travel->destination != elevatorPos)
            {
                ++elevatorPos;
                elevator_level(elevatorPos);
                vTaskDelay(delay);
            }
            functionPtr = unload;
        }
        else if (elevatorPos == MIDDLE)
        {
            elevatorPos = travel->destination;
            elevator_level(elevatorPos);
            vTaskDelay(delay);
            functionPtr = unload;
        }
        else if (elevatorPos == LOWER)
        {
            while (travel->destination != elevatorPos)
            {
                --elevatorPos;
                elevator_level(elevatorPos);
                vTaskDelay(delay);
            }
            functionPtr = unload;
        }
    }
}

void load(struct travel_need *travel)
{
    printf("load");
     printf("\n");
    vTaskDelay(delay);
    functionPtr = move;
}

void unload(struct travel_need *travel)
{
    printf("unload");
     printf("\n");
    vTaskDelay(delay);
    i++;
    functionPtr = idle;
}


void elevator_level(int level){
    if (level == UPPER)
    {
        gpio_set_level(LED_PIN_LEVEL_UP, 1);
        gpio_set_level(LED_PIN_LEVEL_MIDDLE, 0);
        gpio_set_level(LED_PIN_LEVEL_DOWN, 0); 
        printf("UPPER");
        printf("\n");
    } else if(level == MIDDLE)
    {
        gpio_set_level(LED_PIN_LEVEL_UP, 0);
        gpio_set_level(LED_PIN_LEVEL_MIDDLE, 1);
        gpio_set_level(LED_PIN_LEVEL_DOWN, 0);
        printf("MIDDLE");
        printf("\n");
    } else if (level == LOWER)
    {
        gpio_set_level(LED_PIN_LEVEL_UP, 0);
        gpio_set_level(LED_PIN_LEVEL_MIDDLE, 0);
        gpio_set_level(LED_PIN_LEVEL_DOWN, 1); 
        printf("LOWER");
        printf("\n");
    }
    
    
}

void app_main() 
{
    for (size_t j = 0; j < 50; j++)
    {
        test[j].origin = EMPTY;
        test[j].destination = EMPTY;
    }
    

    //Initialize travel needs (50 randomly generated travel needs)
    travel_needs[0].origin = LOWER; travel_needs[0].destination = MIDDLE;
    travel_needs[1].origin = MIDDLE; travel_needs[1].destination = LOWER;
    travel_needs[2].origin = MIDDLE; travel_needs[2].destination = LOWER;
    travel_needs[3].origin = UPPER; travel_needs[3].destination = LOWER;
    travel_needs[4].origin = LOWER; travel_needs[4].destination = MIDDLE;
    travel_needs[5].origin = UPPER; travel_needs[5].destination = LOWER;
    travel_needs[6].origin = MIDDLE; travel_needs[6].destination = LOWER;
    travel_needs[7].origin = MIDDLE; travel_needs[7].destination = UPPER;
    travel_needs[8].origin = UPPER; travel_needs[8].destination = MIDDLE;
    travel_needs[9].origin = MIDDLE; travel_needs[9].destination = UPPER;
    travel_needs[10].origin = MIDDLE; travel_needs[10].destination = LOWER;
    travel_needs[11].origin = UPPER; travel_needs[11].destination = MIDDLE;
    travel_needs[12].origin = UPPER; travel_needs[12].destination = LOWER;
    travel_needs[13].origin = UPPER; travel_needs[13].destination = MIDDLE;
    travel_needs[14].origin = UPPER; travel_needs[14].destination = LOWER;
    travel_needs[15].origin = UPPER; travel_needs[15].destination = MIDDLE;
    travel_needs[16].origin = LOWER; travel_needs[16].destination = MIDDLE;
    travel_needs[17].origin = LOWER; travel_needs[17].destination = MIDDLE;
    travel_needs[18].origin = MIDDLE; travel_needs[18].destination = UPPER;
    travel_needs[19].origin = LOWER; travel_needs[19].destination = MIDDLE;
    travel_needs[20].origin = MIDDLE; travel_needs[20].destination = UPPER;
    travel_needs[21].origin = UPPER; travel_needs[21].destination = MIDDLE;
    travel_needs[22].origin = MIDDLE; travel_needs[22].destination = LOWER;
    travel_needs[23].origin = UPPER; travel_needs[23].destination = LOWER;
    travel_needs[24].origin = LOWER; travel_needs[24].destination = MIDDLE;
    travel_needs[25].origin = MIDDLE; travel_needs[25].destination = UPPER;
    travel_needs[26].origin = MIDDLE; travel_needs[26].destination = LOWER;
    travel_needs[27].origin = UPPER; travel_needs[27].destination = LOWER;
    travel_needs[28].origin = MIDDLE; travel_needs[28].destination = UPPER;
    travel_needs[29].origin = MIDDLE; travel_needs[29].destination = LOWER;
    travel_needs[30].origin = UPPER; travel_needs[30].destination = MIDDLE;
    travel_needs[31].origin = MIDDLE; travel_needs[31].destination = LOWER;
    travel_needs[32].origin = UPPER; travel_needs[32].destination = LOWER;
    travel_needs[33].origin = UPPER; travel_needs[33].destination = LOWER;
    travel_needs[34].origin = MIDDLE; travel_needs[34].destination = LOWER;
    travel_needs[35].origin = LOWER; travel_needs[35].destination = MIDDLE;
    travel_needs[36].origin = UPPER; travel_needs[36].destination = LOWER;
    travel_needs[37].origin = MIDDLE; travel_needs[37].destination = UPPER;
    travel_needs[38].origin = UPPER; travel_needs[38].destination = LOWER;
    travel_needs[39].origin = LOWER; travel_needs[39].destination = MIDDLE;
    travel_needs[40].origin = UPPER; travel_needs[40].destination = MIDDLE;
    travel_needs[41].origin = UPPER; travel_needs[41].destination = MIDDLE;
    travel_needs[42].origin = UPPER; travel_needs[42].destination = MIDDLE;
    travel_needs[43].origin = MIDDLE; travel_needs[43].destination = UPPER;
    travel_needs[44].origin = UPPER; travel_needs[44].destination = LOWER;
    travel_needs[45].origin = LOWER; travel_needs[45].destination = MIDDLE;
    travel_needs[46].origin = LOWER; travel_needs[46].destination = MIDDLE;
    travel_needs[47].origin = LOWER; travel_needs[47].destination = MIDDLE;
    travel_needs[48].origin = UPPER; travel_needs[48].destination = LOWER;
    travel_needs[49].origin = MIDDLE; travel_needs[49].destination = UPPER;



    gpio_config_t config; 


    // Configure pin LED_PIN_LEVEL_UP as output 
    gpio_set_direction(LED_PIN_LEVEL_UP, GPIO_MODE_INPUT_OUTPUT);

    // Configure pin LED_PIN_LEVEL_MIDDLE as output 
    gpio_set_direction(LED_PIN_LEVEL_MIDDLE, GPIO_MODE_INPUT_OUTPUT);

    // Configure pin LED_PIN_LEVEL_DOWN as output 
    gpio_set_direction(LED_PIN_LEVEL_DOWN, GPIO_MODE_INPUT_OUTPUT);

    // Configure pin BUTTON_PIN as input, pull up and with interrupts on the negative edge
    config.pin_bit_mask = (u_int64_t)1<<BUTTON_PIN;
    config.mode = GPIO_MODE_INPUT;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.pull_up_en = GPIO_PULLUP_ENABLE;
    config.intr_type = GPIO_INTR_NEGEDGE;

    esp_err_t res = gpio_config(&config);

    // Activate the interrupts for the GPIOs
    res = gpio_install_isr_service(0);
    ESP_ERROR_CHECK(res);

    // Add a handler to the ISR for pin BUTTON_PIN
    res = gpio_isr_handler_add(BUTTON_PIN, handle_push, NULL);
    ESP_ERROR_CHECK(res);

    functionPtr = idle;

   
    
    // This is where you most likely put your main elevator code. 
    while(1) 
    { 
        if(test[i].origin == EMPTY)    //if(peek(&list, t) == NULL)
        {
            printf("Press the button");
            printf("\n");
            vTaskDelay(delay);
        } else
        {
            functionPtr(&test[i]);
        }
    }
}