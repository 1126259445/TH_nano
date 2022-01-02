#ifndef __USER_DATAPROCESS_H__
#define __USER_DATAPROCESS_H__
#include "app_main.h"

#ifndef DEVECE_ID
#define DEVECE_ID "DEV00001"
#endif

typedef struct __User_data
{
	char allData[1024];
	int dataLen;
} User_data;

extern User_data user_data;

void Task_ParseJSON(void *pvParameters);
void Task_CreatJSON(void *pvParameters);

#endif