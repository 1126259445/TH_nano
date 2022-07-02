/* HTTP GET Example using plain POSIX sockets

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs.h"
#include "nvs_flash.h"

#include <netdb.h>
#include <sys/socket.h>
#include "cJSON.h"
#include "User_HttpRequest_Weather.h"
#include "Dev_Oled_I2c.h"
#include "User_HttpRequest_PublicIp.h"

/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "api.seniverse.com"
#define WEB_PORT 80
#define WEB_URL "https://api.seniverse.com/v3/weather/now.json?key=SJG1TE67YcBqU7D9X&location=shenzhen&language=en&unit=c"

static const char *TAG = "Weather";

static const char *REQUEST = "GET " WEB_URL " HTTP/1.0\r\n"
    "Host: "WEB_SERVER"\r\n"
    "User-Agent: esp-idf/1.0 esp32\r\n"
    "\r\n";


HttpRequest_Weather Http_Weather;

/**************HTTP Weather interface***********************/
HttpRequest_Weather Get_Http_Weather()
{
    return Http_Weather;
}

char* Get_Http_Weather_W()
{
    return Http_Weather.weather;
}

char* Get_Http_Weather_T()
{
    return Http_Weather.temperature;
}


/**********************************************************/
static void Oled_Show_Wrather()
{
    /*show BMP and chinese*/
    if((strstr(Http_Weather.weather,"Sunny")!=NULL) || (strstr(Http_Weather.weather,"Fair")!=NULL) \
       || (strstr(Http_Weather.weather,"sunny")!=NULL) || (strstr(Http_Weather.weather,"fair")!=NULL))
    {
        OLED_DrawBMP(0,0,32,4,Sunny);
        OLED_ShowChinese(0,4,chinese[0]);
        OLED_ShowString(16,4,"  ",SIZE16);
    }
    else if((strstr(Http_Weather.weather,"Clear")!=NULL) || (strstr(Http_Weather.weather,"clear")!=NULL))
    {
        OLED_DrawBMP(0,0,32,4,Clear);
        OLED_ShowChinese(0,4,chinese[0]);
        OLED_ShowString(16,4,"  ",SIZE16);
    }
    else if((strstr(Http_Weather.weather,"Cloudy")!=NULL) || (strstr(Http_Weather.weather,"Overcast")!=NULL) \ 
        || (strstr(Http_Weather.weather,"cloudy")!=NULL) || (strstr(Http_Weather.weather,"overcast")!=NULL))
    {
        OLED_DrawBMP(0,0,32,4,Cloudy);
        OLED_ShowChinese(0,4,chinese[6]);
        OLED_ShowChinese(16,4,chinese[7]);
    }
    else if((strstr(Http_Weather.weather,"Rain")!=NULL) || (strstr(Http_Weather.weather,"Storm")!=NULL) \
     || (strstr(Http_Weather.weather,"rain")!=NULL) || (strstr(Http_Weather.weather,"storm")!=NULL))
    {
         OLED_DrawBMP(0,0,32,4,Rain);

        if(strstr(Http_Weather.weather,"Light")!=NULL)
        {
            OLED_ShowChinese(0,4,chinese[1]);
            OLED_ShowChinese(16,4,chinese[5]);
        }
        else if(strstr(Http_Weather.weather,"Moderate")!=NULL)
        {
            OLED_ShowChinese(0,4,chinese[2]);
            OLED_ShowChinese(16,4,chinese[5]);
        }
        else if(strstr(Http_Weather.weather,"Heavy")!=NULL)
        {
            OLED_ShowChinese(0,4,chinese[3]);
            OLED_ShowChinese(16,4,chinese[5]);
        }
        else
        {
            OLED_ShowChinese(0,4,chinese[4]);
            OLED_ShowChinese(16,4,chinese[5]);
        }
    }
    else if((strstr(Http_Weather.weather,"Snow")!=NULL) || (strstr(Http_Weather.weather,"snow")!=NULL))
    {
        OLED_DrawBMP(0,0,32,4,Snow);
        if(strstr(Http_Weather.weather,"Light")!=NULL) 
        {
            OLED_ShowChinese(0,4,chinese[1]);
            OLED_ShowChinese(16,4,chinese[10]);
        }
        else if(strstr(Http_Weather.weather,"Moderate")!=NULL) 
        {
            OLED_ShowChinese(0,4,chinese[2]);
            OLED_ShowChinese(16,4,chinese[10]);
        }
        else if(strstr(Http_Weather.weather,"Heavy")!=NULL) 
        {
            OLED_ShowChinese(0,4,chinese[3]);
            OLED_ShowChinese(16,4,chinese[10]);
        }
        else if(strstr(Http_Weather.weather,"Snowstorm")!=NULL) 
        {
            OLED_ShowChinese(0,4,chinese[4]);
            OLED_ShowChinese(16,4,chinese[10]);
        }
    }
    else if((strstr(Http_Weather.weather,"Shower")!=NULL) || (strstr(Http_Weather.weather,"shower")!=NULL))
    {
        OLED_DrawBMP(0,0,32,4,Shower);
        if((strstr(Http_Weather.weather,"Thundershower")!=NULL) || (strstr(Http_Weather.weather,"thundershower")!=NULL))
        {
            OLED_ShowChinese(0,4,chinese[8]);
            OLED_ShowChinese(16,4,chinese[5]);
        }
        else
        {
            OLED_ShowChinese(0,4,chinese[12]);
            OLED_ShowChinese(16,4,chinese[5]);
        }
    }
    else if((strstr(Http_Weather.weather,"Windy")!=NULL) || (strstr(Http_Weather.weather,"Blustery")!=NULL) || (strstr(Http_Weather.weather,"Hurricane")!=NULL) \
        || (strstr(Http_Weather.weather,"windy")!=NULL) || (strstr(Http_Weather.weather,"blustery")!=NULL) || (strstr(Http_Weather.weather,"hurricane")!=NULL))
        {
            OLED_DrawBMP(0,0,32,4,Windy);
            OLED_ShowChinese(0,4,chinese[3]);
            OLED_ShowChinese(16,4,chinese[9]);
        }
    else 
        OLED_DrawBMP(0,0,32,4,Sunny);

    /*show temperaure*/
    OLED_ShowString(32,4,Http_Weather.temperature,SIZE16);

    OLED_ShowString(32,0,"  ",SIZE16);OLED_ShowString(32,2,"  ",SIZE16);

}

//Json_return data {"results":[{"location":{"id":"WS10730EM8EV","name":"深圳","country":"CN","path":"深圳,深圳,广东,中国","timezone":"Asia/Shanghai","timezone_offset":"+08:00"},"now":{"text":"多云","code":"4","temperature":"22"},"last_update":"2021-11-13T11:11:52+08:00"}]}
static uint8_t Http_Data_process(char *recv_buf)
{
     cJSON *root;
	////首先整体判断是否为一个json格式的数据
	root = cJSON_Parse(recv_buf);
	//如果是否json格式数据
	if (root == NULL)
	{
		printf("\r\n[SY] Http_Data_process not json ... \r\n");
		cJSON_Delete(root);
		return 0;
	}
    else{
        cJSON *result_arr = cJSON_GetObjectItem(root, "results");
        cJSON *arr0 = cJSON_GetArrayItem(result_arr,0);
        cJSON *now = cJSON_GetObjectItem(arr0,"now");
        char *weather = cJSON_GetObjectItem(now,"text")->valuestring;
        char *temperature = cJSON_GetObjectItem(now,"temperature")->valuestring;

        memcpy(Http_Weather.weather,weather,strlen(weather));
        memcpy(Http_Weather.temperature,temperature,strlen(temperature));

        printf("\r\nHTTP Weather >>>>>>>>>>>\r\n %s   %s  \r\n>>>>>>>>>>>>>>>>>>\r\n",Http_Weather.weather,Http_Weather.temperature);
        Oled_Show_Wrather();
        return 1;
    }
    cJSON_Delete(root);
    return 0;
}

static void New_RequestUrl(char *buf)
{
    sprintf(buf,"GET https://api.seniverse.com/v3/weather/now.json?key=SJG1TE67YcBqU7D9X&location=%s&language=en&unit=c HTTP/1.0\r\n" \
    "Host: api.seniverse.com\r\n" \
    "User-Agent: esp-idf/1.0 esp32\r\n" \
    "\r\n",Http_Pos.city);
}

static void Task_HttpRequestWeather(void *pvParameters)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s, r;
    char recv_buf[256] = {0};

    char all_buf[5120] = {0};
    uint16_t count = 0;

    while(1)
    {
        while(Http_Pos.city == NULL) 
        {
            vTaskDelay(1000);
            ESP_LOGE(TAG, "Waiting POS");
        }

        int err = getaddrinfo(WEB_SERVER, "80", &hints, &res);

        if(err != 0 || res == NULL) {
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        /* Code to print the resolved IP.

            Note: inet_ntoa is non-reentrant, look at ipaddr_ntoa_r for "real" code */
        addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
        ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

        s = socket(res->ai_family, res->ai_socktype, 0);
        if(s < 0) {
            ESP_LOGE(TAG, "... Failed to allocate socket.");
            freeaddrinfo(res);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... allocated socket");

        if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
            close(s);
            freeaddrinfo(res);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(TAG, "... connected");
        freeaddrinfo(res);
 
        char request_url[512] = {0};
        ESP_LOGI(TAG, request_url);
        New_RequestUrl(request_url);
        while (write(s, request_url, strlen(request_url)) < 0) {
            ESP_LOGE(TAG, "... socket send failed");
            close(s);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... socket send success");

        struct timeval receiving_timeout;
        receiving_timeout.tv_sec = 5;
        receiving_timeout.tv_usec = 0;
        if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
                sizeof(receiving_timeout)) < 0) {
            ESP_LOGE(TAG, "... failed to set socket receiving timeout");
            close(s);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... set socket receiving timeout success");

        /* Read HTTP response */
        count = 0;
        do {
            bzero(recv_buf, sizeof(recv_buf));
            r = read(s, recv_buf, sizeof(recv_buf)-1);
          //  for(int i = 0; i < r; i++) {
          //      putchar(recv_buf[i]);
          //  }

            if((count+r) < sizeof(all_buf))
            {
                memcpy((void*)&all_buf[count],(void*)recv_buf,r);
                count += r;
            }   
            else
            {
                break;
            }

        } while(r > 0);
        
        char *data_ok = strstr((const char *) all_buf,(const char *)"200 OK");
        char *data_pt = strstr((const char *) all_buf,(const char *)"results");
        if(data_ok != NULL && data_pt != NULL)
        {
            if(Http_Data_process(data_pt-2))
            {
                 vTaskDelay(600000 / portTICK_PERIOD_MS);
            }
        }
        ESP_LOGI(TAG, "\r\n... done reading from socket. Last read return=%d errno=%d\r\n", r, errno);
        close(s);
         vTaskDelay(1000 / portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "Http_Request Starting again!\r\n");
    }
}


void HTTP_Weather_Init()
{
    int ret = pdFAIL;
    ret = xTaskCreate(Task_HttpRequestWeather, "Task_HttpRequestWeather", 1024*10, NULL, 5, NULL);
    if (ret != pdPASS)
    {
        printf("create Task_HttpRequestTime thread failed.\n");
    }
}
