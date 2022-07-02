#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"
#include "driver/uart.h"

#include "esp_log.h"
#include "esp_system.h"
#include "driver/adc.h"
#include "Dev_Ntc.h"
#include "math.h"





static uint16_t Get_Adc_Average(void)
{
    uint16_t adc_data[ADC_SAMPLE_NUM]  = {0};
    uint32_t adc_sum = 0;
    if (ESP_OK == adc_read_fast(adc_data, ADC_SAMPLE_NUM)) 
    {
        for (uint8_t x = 0; x < ADC_SAMPLE_NUM; x++) 
        {
            adc_sum += adc_data[x];
        }
    }
    return (uint16_t)(adc_sum/ADC_SAMPLE_NUM);
}



//电压换算成温度
float Get_Ntctemp_Data(void)
{
    float temp = 0;
    float vol=0;
    float Rt=0;
    float T2=273.15+25;
    float Ka=273.15;

    vol=(float)((Get_Adc_Average()*REF_VOLT)/ADC_ACCURACY);
    Rt = (vol * SERIES_R) / (VCC_VOLT-vol);
    temp=1/(1/T2+log(Rt/NTC_RP)/NTC_B)-Ka+0.5;

    return temp;
}


void Dev_Ntc_ADC_Init(void)
{
    // 1. init adc
    adc_config_t adc_config;

    // Depend on menuconfig->Component config->PHY->vdd33_const value
    // When measuring system voltage(ADC_READ_VDD_MODE), vdd33_const must be set to 255.
    adc_config.mode = ADC_READ_TOUT_MODE;
    adc_config.clk_div = 8; // ADC sample collection clock = 80MHz/clk_div = 10MHz
    ESP_ERROR_CHECK(adc_init(&adc_config));
}
