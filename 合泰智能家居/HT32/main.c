//pb1,pb2Ϊ8266��tx��rx�ӿ�
//pc2��pe3��pb3Ϊ���0��1��2�����ݿ�
//pa7��pa6Ϊ���Դ��ڵ�tx��rx
//pa3Ϊ����mq-2�����ݿ�
//pa4Ϊdht11�����ݿ�
#include "UART.h"
#include "HT66F2390.h"
#include "delay.h"
#include "DHT11.h"
#include "MQ.h"
#include "DUOJI.h"
#include "ESP.h"  
extern u8 Temp,Humid,TL,rec_dat[17];
	
void main()
{	
//	u32 i;

	_wdtc = 0b10101111;
	UART_SET();
	esp8266_init();
	duoji_start0();
	duoji_start1();
	duoji_start2();
	delay_ms(1500);
	ESP8266_ModeInit();	
	DHT11_time_init();
	while(!_hircf);
	while(1);
	{	
/*		send_char('a');*/
//		duoji1(90);
//		delay_ms(1500);
//		duoji1(180);
//	send_char('A');
/*	MQ_2();*/
//	DHT11_receive();
//	delay_ms(300);	
	}
		
}
