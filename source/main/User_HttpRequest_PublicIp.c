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

#include <netdb.h>
#include <sys/socket.h>
#include "cJSON.h"
#include "User_HttpRequest_PublicIp.h"

/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "restapi.amap.com"
#define WEB_PORT 80
#define WEB_URL "https://restapi.amap.com/v3/ip?ip=%s&output=JSON&key=d07cd5a8d9cdcc19334ad8c8e9c4a73f"

static const char *TAG = "PublicIp";
TaskHandle_t PosTask_Handler = NULL;			//task1任务句柄

static const char *REQUEST = "POST " WEB_URL " HTTP/1.0\r\n"
     "Host: "WEB_SERVER"\r\n"
    "User-Agent: esp-idf/1.0 esp32\r\n"
    "\r\n";


HttpRequest_Pos Http_Pos;

static char PublicIp[20] = {0};
/**************HTTP Weather interface***********************/
char* Get_Http_PublicIp()
{
    return PublicIp;
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
        char *province = cJSON_GetObjectItem(root, "province")->valuestring;
        char *city = cJSON_GetObjectItem(root,"city")->valuestring;
        char *adcode = cJSON_GetObjectItem(root,"adcode")->valuestring;
        char *rectangle = cJSON_GetObjectItem(root,"rectangle")->valuestring;

        memcpy(Http_Pos.province,province,strlen(province));
        memcpy(Http_Pos.city,city,strlen(city));
        memcpy(Http_Pos.adcode,adcode,strlen(adcode));
        memcpy(Http_Pos.rectangle,rectangle,strlen(rectangle));
        printf("HTTP POS>>>>>>>>>>>>>>>>>>>>>\r\n");
        printf("%s %s %s %s",Http_Pos.province,Http_Pos.city,Http_Pos.adcode,Http_Pos.rectangle);
        printf("\r\n>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");
        return 1;
    }
    cJSON_Delete(root);
    return 0;
}


static void Task_HttpRequestPublicIp(void *pvParameters)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s, r;
    char recv_buf[64] = {0};
    char all_buf[1024] = {0};
    uint16_t count = 0;

    while(1)
    {
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
 

        while (write(s, REQUEST, strlen(REQUEST)) < 0) {
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
        bzero(all_buf, sizeof(all_buf));
        do {
            bzero(recv_buf, sizeof(recv_buf));
            r = read(s, recv_buf, sizeof(recv_buf)-1);

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
        char *data_pt = strstr((const char *) all_buf,(const char *)"status");
        if(data_ok != NULL && data_pt != NULL)
        {
            ESP_LOGI(TAG, "HTTP/1.1 200 OK");
            if(Http_Data_process(data_pt-2) == 1)
            {
                vTaskDelete(PosTask_Handler);
            }
        }
        ESP_LOGI(TAG, "\r\n... done reading from socket. Last read return=%d errno=%d\r\n", r, errno);
        close(s);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
    }
}


void HTTP_PublicIp_Init()
{
    int ret = pdFAIL;
    ret = xTaskCreate(Task_HttpRequestPublicIp, "Task_HttpRequestPublicIp", 1024*2, NULL, 5, &PosTask_Handler);
    if (ret != pdPASS)
    {
        printf("create Task_HttpRequestPublicIp thread failed.\n");
    }
}
