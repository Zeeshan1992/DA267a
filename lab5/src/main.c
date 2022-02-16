#include "i2c_drivers.h"
#include "circular_buffer.h"
#include "esp_sleep.h"
#include "esp_pm.h"
#include <math.h>
#include <esp_task_wdt.h>

/**
 * Author 
 * Daniel Lone DL
 * Abdullahi Farah AF
 * 
 */
#define SCL 32
#define SDA 33
#define BUTTON_PIN 1
#define LED_PIN 2

#define PUSH_TIME_US 250000// 250 ms

/**
 * We wanted to collect 5 samples during half a second so that we had 
 * 5 samples to read every 500 ms which the period we decided for the algo period 
 */
#define SAMPLING_PERIOD 100
#define ALGO_PERIOD 500

#define BUFF_SIZE 5
// minimum SD to avoid converging to 0
#define MIN_SD 1
// constant applied to SD to detect steps
#define K 1
// minimum time between steps, this value is chosen because...
#define MIN_INTRA_STEP_TIME 300
// daily goal of steps
#define STEPS_GOAL 10 //small value just for testing

static volatile uint64_t lastPush = -PUSH_TIME_US;
int step_count = 0;

#define SAMPLE_PRIORITY 5
#define ALGO_PRIORITY 6

struct circularBuffer cbuffer;

SemaphoreHandle_t xSemaphore = NULL;


#define MPU6050_ADDR 0x68 //If AD0 is LOW, the I2C address of the board will be 0x68. Otherwise, the address will be 0x69.

/*
From Datasheet, page 40: power managment register
Register (Hex): 6B
Bit7 DEVICE_RESET
Bit6 SLEEP
Bit5 CYCLE
Bit4 -
Bit3 TEMP_DIS
Bit2, 
Bit1, 
Bit0 CLKSEL[2:0]
*/
#define MPU6050_PWR_MGMT_1 0x6B 

#define MPU6050_SMPLRT_DIV 0x19

#define ACC_X_H 0x3B
#define ACC_X_L 0x3C
#define ACC_Y_H 0x3D
#define ACC_Y_L 0x3E
#define ACC_Z_H 0x3F
#define ACC_Z_L 0x40

void waitMs(unsigned int millis) //AF
{
    TickType_t delay = millis / portTICK_PERIOD_MS;
    vTaskDelay(delay);
}

void blinkLED(int ms, int times) //AF
{
    for (int i = 0; i < times; i++)
    {
        gpio_set_level(LED_PIN, 1);
        waitMs(ms);
        gpio_set_level(LED_PIN, 0);
        waitMs(ms);
    }
}

static void sampling_task(void *arg) //DL
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    // place the while loop from main here
    while (1)
   {
       uint8_t buffer;
       uint16_t x = 0;
       uint16_t y = 0;
       uint16_t z = 0;
    
       readI2C(MPU6050_ADDR,ACC_X_L, &buffer, 1); // read the low bit of x
       x = buffer;
       readI2C(MPU6050_ADDR,ACC_X_H, &buffer, 1); // now the high bit
       x |= ((int16_t)buffer << 8);  // combining low and high into a 16 bit signed integer

       readI2C(MPU6050_ADDR,ACC_Y_L, &buffer, 1); //read the low bit of y
       y = buffer;
       readI2C(MPU6050_ADDR,ACC_Y_H, &buffer, 1); // now the high bit
       y |= ((int16_t)buffer << 8); // combining low and high into a 16 bit signed integer

       readI2C(MPU6050_ADDR,ACC_Z_L, &buffer, 1); //read the low bit of z
       z = buffer;
       readI2C(MPU6050_ADDR,ACC_Z_H, &buffer, 1); // now the high bit
       z |= ((int16_t)buffer << 8); // combining low and high into a 16 bit signed integer

       uint16_t acc = sqrt((x*x) + (y*y) + (z*z));


       addElement(&cbuffer,acc);

       vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SAMPLING_PERIOD)); // place inside while loop at the end
   }
    
}

static void algo_task(void *arg) // DL + AF
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
   
    
    while (1)
    {
        int size = cbuffer.count;
        float m = 0;
        float sd = 0;
        int m1 = 0;
        int m2 = 0;
        double sd1 = 0;
        double sd2 = 0;
        int sample = 0;
        int compare = 0;

        if (size > 0)
        {
            // compute mean here do not empty the queue
            for (int i = 0; i < size; i++)
            {
                m1 = getHead(&cbuffer);
                m2 += m1;
            }
            m = m2 / size;
            resetHead(&cbuffer);

            // compute sd, do not empty the queue
            for (int i = 0; i < size; i++)
            {
                sd1 = getHead(&cbuffer);
                sd2 += pow((sd1 - m), 2);
            }
            sd = sqrt(sd2 / size);
            resetHead(&cbuffer);

            if (sd < MIN_SD)
            {
                sd = MIN_SD;
            }
            //now do step counting, while also emptying the queue
            for (int i = 0; i < size; i++)
            {
                sample = removeHead(&cbuffer);
                compare = m + (K * sd);
                if (sample > compare)
                {
                    step_count ++;
                    printf("step %d\n", step_count);
                }
            }
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ALGO_PERIOD));
    }
    

}

/** 
 * button pressed ISR
 * also handeling debouncing
 */
void button_isr_handler(void *arg) //AF
{
    uint64_t now = esp_timer_get_time();
    if ((now - lastPush) > PUSH_TIME_US)
    {
        lastPush = now;
        // give semaphore(this is in an ISR!)
        xSemaphoreGiveFromISR(xSemaphore, NULL);
    }
}

void led_task(void *arg)  //AF
{
    while(1)
    {
        // wait for semaphore
        if(xSemaphoreTake(xSemaphore, portMAX_DELAY) ==pdTRUE)
        {
            // flash LED with sequence depending on if step_count > STEPS_GOAL
            if (step_count > STEPS_GOAL)
            {
                blinkLED(10, 10);
            }
            else
            {
                blinkLED(1000,2);
            }
            
            
        }
        else
        {
            // Do nothing
        }
    }
}


void app_main() //DL + AF
{ 
    int *buffer_data = (int*) malloc(BUFF_SIZE * sizeof(int));
    initCircularBuffer(&cbuffer, buffer_data, BUFF_SIZE);
    
    esp_pm_config_esp32_t pm_config = {
        .light_sleep_enable = true,
        .max_freq_mhz = 80,
        .min_freq_mhz = 16
    };
    
    esp_pm_configure(&pm_config);

    initI2C(SDA, SCL);

    writeI2C(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x00); // setting the power mode of the accelerometer, writing 0 selects 8MHz frequency as the clock source
    /**
     * setting the sampling frequency
     * sampling frequency is gyro sampling freq / (1 + divider)
     */
    writeI2C(MPU6050_ADDR, MPU6050_SMPLRT_DIV, 250); 

    // create binary semaphore
    xSemaphore = xSemaphoreCreateBinary();
    // task for the led
    xTaskCreate(led_task, "led", 2048, NULL, 10, NULL);

    // create sampling task
    xTaskCreate(sampling_task, "sampling", 2048, NULL, SAMPLE_PRIORITY, NULL);

    // create algorithm task
    xTaskCreate(algo_task, "algo", 2048, NULL, ALGO_PRIORITY, NULL);

    // configure button and led pinsg
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // setup the btn with an internal pullup
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    // enable interrupt on falling (1->0) edge for button pin
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);

    // install ISR service with default configuration
    gpio_install_isr_service(0);

    // attach the interrupt service routine
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

}