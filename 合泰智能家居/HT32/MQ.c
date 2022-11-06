#include "HT66F2390.h"
#include "MQ.h"
#include "delay.h"
#include "UART.h"
#include "ESP.h"  
#include "DUOJI.h"
#define MQ  _pa3
#define MQC  _pac3


void MQ_2()
{
	MQC=1;
	delay_ms(50);
	if(MQ==1)
	{
		GCC_DELAY(10);
		if(MQ==1)
		{	
	   	ESP8266_SendCmd("no polluted");	
		send_char('\r');
		send_char('\n');
		}				
	}
	
	if(MQ==0)
	{
		GCC_DELAY(10);
		if(MQ==0)
		{
	   	ESP8266_SendCmd("polluted");	
		send_char('\r');
		send_char('\n');
		duoji1(180);
		}			
	}
}