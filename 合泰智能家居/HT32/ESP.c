#include "HT66F2390.h"
#include "UART.h"
#include "delay.h"
#include "duoji.h"
#include "ESP.h"
#define LED1  _ph0
#define LED2  _ph1
#define LED1C  _phc0
#define LED2C  _phc1
int len2;
int len0;
int data_count2=0,u=0; 
char RecBuf2[50],door;//u8��ʾchar  u16��ʾint
u8 RecBuf0[50];
void esp8266_init()
{
	_pbs0 = _pbs0 | 0b11101100;//pb1:tx,pb2:rx,pb3;ptp2
	_u2cr1 = 0b10000000;
    _u2cr2 = 0b11101100;
    _ur2e = 1;

     _brg2 = 51;  //������ 9600
    _mf5e = 1;
    _emi =1;
}
void send_char(char s)
{
/*	while(_u2sr==0);*/
    _txr_rxr2 =s;
    while(_tidle2==0);
}
//void send_buff(char *s)
//{
//    while(*s)
//    {
//        UART_Send_Byte(*s);
//        s++;
//    }    
//}
int re_buff(char *rbuff,char c)
{ 
	char rdata;//���ݳ���
    while(_ridle2 == 0); //�ȴ��������
    _emi=0;
    rdata =_txr_rxr2;
    if(rdata!=c)
    {
        rbuff[data_count2]=rdata;
        data_count2++;
    }     
     else
     {
         rbuff[data_count2]='\0';
         len2=data_count2;
            data_count2=0; 
            return len2;
     }
     _emi=1;
    return data_count2;
}
int re_buff0(char *rbuff,char c)
{ 
	char rdata;
	int data_count0=0;    //���ݳ���
    while(_ridle0 == 0); //�ȴ��������
    _emi=0;
    rdata =_txr_rxr0;
    if(rdata!=c)
    {
        rbuff[data_count0]=rdata;
        data_count0++;
    }     
     else
     {
         rbuff[data_count0]='\0';
         len0=data_count0;
            data_count0=0; 
            return len0;
     }
     _emi=1;
    return data_count0;
}
void ESP8266_SendCmd(char *pbuf)
{
	while(*pbuf!='\0') //�����ո�����ѭ��	
	{
		send_char(*pbuf);
		GCC_DELAY(10);//�ӳ�5us,�Լ�Ūһ�º���ģ��
		pbuf++;	
	}
	GCC_DELAY(100);
	send_char('\r');//�س�
	GCC_DELAY(100);
	send_char('\n');//����
	delay_ms(1000);
}


void ESP8266_ModeInit(void)
{
	ESP8266_SendCmd("AT+RST");
	delay_ms(1000);
	ESP8266_SendCmd("AT+CWMODE=1");//����·����ģʽ
	delay_ms(1000);
	ESP8266_SendCmd("AT+CWJAP=\"jj\",\"12345678\""); 
	delay_ms(10000);
//	ESP8266_SendCmd("AT+RST");//��������wifiģ�飨��ʡ�ԣ�
	ESP8266_SendCmd("AT+CIPMUX=0");	//����������ģʽ
	delay_ms(1000);
	ESP8266_SendCmd("AT+CIPSTART=\"TCP\",\"192.168.69.125\",8888");	//���ӷ�����
	delay_ms(1000);
	ESP8266_SendCmd("AT+CIPMODE=1");	//͸��ģʽ
	delay_ms(1000);	
	ESP8266_SendCmd("AT+CIPSEND");	//���ݴ���ģʽ
	delay_ms(1000);
}

DEFINE_ISR(UART0,0x3C)
{	
		LED1C=0;  //�O��LED1��ݔ��ģʽ
		LED2C=0;  //�O��LED2��ݔ��ģʽ
		_pgc2=0;
		_pgc4=0;
		int buff_length0,buff_length2;
        buff_length2=re_buff(RecBuf2,'\n')+48;  //���ȴ���9����ʾ����ĳ���ֵ��ԭ�����ASCIIת��
       /* buff_length0=re_buff0(RecBuf0,'\n')+48;*/
//        if(data_count0==0) 
//        {
//        	UART_Send_Byte(RecBuf0);
//        }
		if(_ur0f==1) //HT66�յ�ָ�����������󣬷���������ҷ��͸��ֻ��� open door
		{
			ESP8266_SendCmd("The door was opened");
			duoji1(0);
		}
        if(_ur2f==1)//1:Զ�̴��š�2����������3������ 
        { 
        	if(RecBuf2[u]=='1')//RecBuf[9]�൱��������"+IPD��x,x:"��0��8��9���ַ�
			{
				LED1=1;
				send_char('1');
				duoji0(45);
				duoji1(20);
				duoji2(20);
				_pg2=1;
				_pg4=1;
			}
			if(RecBuf2[u]=='2')//RecBuf[9]�൱��������"+IPD��x,x:"��0��8��9���ַ�
			{	
				LED1=0;
				send_char('2');
				duoji0(90);
				duoji1(90);
				duoji2(90);
			}
			if(RecBuf2[u]=='3')//RecBuf[9]�൱��������"+IPD��x,x:"��0��8��9���ַ�
			{
				LED2=0;
				send_char('3');
				duoji0(120);
				duoji1(120);
				duoji2(120);
			}
			if(RecBuf2[u]=='4')//RecBuf[9]�൱��������"+IPD��x,x:"��0��8��9���ַ�
			{
				LED2=1;
				UART_Send_Byte('4');
				duoji0(180);
				duoji1(180);
				duoji2(180);
				_pg2=0;
				_pg4=0;
			}
			u++;
        }
    _ur2f = 0;  //�����־
    _ur0f = 0;  //�����־
//    _mf5f=0;
//    _emi=0;

}
