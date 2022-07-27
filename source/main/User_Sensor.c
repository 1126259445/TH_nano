#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"
#include "driver/uart.h"

#include "esp_log.h"
#include "esp_system.h"
#include "string.h"

#include "Dev_Dht11.h"
#include "Dev_Oled_I2c.h"
#include "Dev_Ntc.h"

#include "User_HttpRequest_Time.h"
#include "User_HttpRequest_Weather.h"

TaskHandle_t Sensor_Handle = NULL;


/**
 * @description: Sensor Read_data
 * @param {type} 
 * @return: 
 */
void Task_Sensor(void *pvParameters)
{
    printf("Task_Sensor------------------------------------------ \r\n");
    static uint32_t Dht11_Tick = 0;
    static uint32_t Time_Weather_Tick = 0;

    Dht11_Init();

    while(1)
    {
        if(++Dht11_Tick > 1)
        {
            Dht11_Tick = 0;
            printf("read Dht11 \r\n");
            uint8_t ret = Read_Dht11_Data();
            printf("read NTC \r\n");
            float Ntc_Temperature = Get_Ntctemp_Data();

            char Temperature[10] = {0};
            char Humidity[10] = {0};
            float f_Temperature = Get_Dht11_Temperature()/10.0f;
            float f_Humidity = Get_Dht11_Humidity()/10.0f;

            if(f_Humidity != 0)
            {
                sprintf(Temperature,"T:%.1f",f_Temperature); 
                sprintf(Humidity,"H:%.1f%%",f_Humidity);  
            }
            else
            {
                sprintf(Temperature,"T:%.1f",Ntc_Temperature);
                sprintf(Humidity,"H:---");
            }
            
            printf("%s,%s\r\n",Temperature,Humidity);
            OLED_ShowString(0,6,Temperature,SIZE16);
            OLED_ShowString(72,6,Humidity,SIZE16);     
        }

        if(++Time_Weather_Tick > 3)
        {
            Time_Weather_Tick = 0;
            Oled_Show_Wrather();
            Oled_Show_Time();
        }

         vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}



