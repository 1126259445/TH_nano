#ifndef __NTC_H__
#define __NTC_H__
/*
NTC 热敏电阻温度计算公式：Rt = R EXP(B(1/T1-1/T2))

其中，T1和T2指的是K度，即开尔文温度。

Rt 是热敏电阻在T1温度下的阻值。

R是热敏电阻在T2常温下的标称阻值。100K的热敏电阻25℃的值为10K（即R=10K）。T2=(273.15+25)

EXP是e的n次方

B值是热敏电阻的重要参数

通过转换可以得到温度T1与电阻Rt的关系T1=1/（ln（Rt/R）/B+1/T2），这里可以将ln换算成log，即T1=1/（log（Rt/R）/B+1/T2）。

对应的摄氏温度t=T1-273.15，同时+0.5的误差矫正。
*/
#define ADC_SAMPLE_NUM  20

#define VCC_VOLT    3300    //3300mV
#define REF_VOLT    1000    //esp8266  = 1000mV
#define NTC_B       3950    //NTC B
#define NTC_RP      10000    //NTC 25 10K
#define SERIES_R    2000    //2K    33000   //33K
#define ADC_ACCURACY  1024



void Dev_Ntc_ADC_Init(void);
float Get_Ntctemp_Data(void);

#endif
