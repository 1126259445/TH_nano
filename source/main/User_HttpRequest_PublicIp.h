#ifndef __HTTP_PUBLICIP_H__
#define __HTTP_PUBLICIP_H__

typedef struct 
{
    char province[20];
    char city[20];
    char adcode[20];
    char rectangle[100];
}HttpRequest_Pos;
extern HttpRequest_Pos Http_Pos;

char* Get_Http_PublicIp(void);
void HTTP_PublicIp_Init(void);

#endif
