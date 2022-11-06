#include "HT66F2390.h"
#include "MQ-2.h"
#include "delay.h"
#include "UART.h"
#define MQ  _pa3
#define MQC  _pac3
#define LED1  _ph1
#define LED1C  _phc1


void MQ_2()
{
	MQ=0;
	MQC=1;
	LED1=0;
	LED1C=0;
	if(MQ==0)
	{
		GCC_DELAY(10);
		if(MQ==0)
		{
			LED1=1;	
		}	
	}
}