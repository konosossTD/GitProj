#ifndef __8266_H 
#define __8266_H 

#include "HT66F2390.h"
#define u8 unsigned char
#define u32 unsigned int
#define u16 unsigned short
void esp8266_init();
void send_char(char s);
int re_buff(char *rbuff,char c);
void ESP8266_SendCmd(char *pbuf);
void ESP8266_ModeInit(void);
#endif