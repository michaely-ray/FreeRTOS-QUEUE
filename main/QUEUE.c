#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

const int SET_GPIO = 2;

xQueueHandle queue;


void handleWifi(void *params)
{
    int countLed = 0;

    while (true)
    {
        countLed ++;
        gpio_pad_select_gpio(SET_GPIO);
        gpio_set_direction(SET_GPIO, GPIO_MODE_OUTPUT);
        gpio_set_level((gpio_num_t)SET_GPIO, 0);
        vTaskDelay(2000/ portTICK_PERIOD_MS);
        printf("GET IP \n\n"); 
        printf("MQTT Start\n\n");
        xQueueSend(queue, &countLed,2000 / portTICK_PERIOD_MS);
    }
}



void statusLed(void *params)
{
    int rxInt;
    while (true)
    // xQueueReceive(queue, &rxInt , 5000 / portTICK_PERIOD_MS);
    
    // if(rxInt!= NULL){
    if(xQueueReceive(queue, &rxInt , portMAX_DELAY)){
        gpio_pad_select_gpio(SET_GPIO);
        gpio_set_direction(SET_GPIO, GPIO_MODE_OUTPUT);
        gpio_set_level((gpio_num_t)SET_GPIO, 1);
        printf("\n %d\n", rxInt);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        }

    // else{
    //     gpio_pad_select_gpio(SET_GPIO);
    //     gpio_set_direction(SET_GPIO, GPIO_MODE_OUTPUT);
    //     gpio_set_level((gpio_num_t)SET_GPIO, 0);
    //     printf("\n Wifi não conectado\n");
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }

}

void app_main(void)
{
    queue = xQueueCreate(3, sizeof(int));
    xTaskCreate(&handleWifi, "handleWIFI", 2048, NULL, 2, NULL);
    xTaskCreate(&statusLed, "MQTT Connected", 2048, NULL, 2, NULL);
  
}