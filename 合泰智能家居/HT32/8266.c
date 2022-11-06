#include "HT66F2390.h"
#include "UART.h"
#include "delay.h"
#include "duoji.h"
void esp8266_init()
{
	_pbs0 = _pbs0 | 0b11110000;//pb1:tx,pb2:rx
	_u2cr1 = 0b10000000;
    _u2cr2 = 0b11101100;
    _ur2e = 1;

     _brg2 = 207;  //������ 2400
    _mf5e = 1;
    _emi =1;
}
void send_char(char s)
{
    _txr_rxr2 =s;
    while(_tidle2 == 0 );
}
int re_buff(char *rbuff,char c)
{ char rdata;
int len;
int data_count=0;
    while(_ridle2 == 0); //�ȴ��������
    rdata =_txr_rxr2;
    if(rdata!=c)
    {
        rbuff[data_count]=rdata;
        data_count++;
    }     
     else
     {
         rbuff[data_count]='\0';
         len=data_count;
            data_count=0; 
            return len;
     }
    return data_count;
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
	GCC_DELAY(1000);
}

void ESP8266_ModeInit(void)
{
	ESP8266_SendCmd("AT+RST");
	delay_ms(1000);
	ESP8266_SendCmd("AT+CWMODE=1");//����·����ģʽ
	delay_ms(1000);
	ESP8266_SendCmd("AT+CWJAP=\"zjj\",\"12345678\""); 
	delay_ms(10000);
//	ESP8266_SendCmd("AT+RST");//��������wifiģ�飨��ʡ�ԣ�
	ESP8266_SendCmd("AT+CIPMUX=1");	//����������ģʽ�����������ͻ��˽���
	delay_ms(1000);
	ESP8266_SendCmd("AT+CIPSERVER=1,8899");	//����TCP/IP �˿�Ϊ8080 
	delay_ms(1000);	
}
//DEFINE_ISR(UART2,0x3C)
//{		
//		char RecBuf[50];
//		int data_count=0;
//		int buff_length;
//        buff_length=re_buff(RecBuf,'\n')+48;  //���ȴ���9����ʾ����ĳ���ֵ��ԭ�����ASCIIת��
//        if(data_count==0) 
//        { 
//        	if(RecBuf[9]=='1')//RecBuf[9]�൱��������"+IPD��x,x:"��0��8��9���ַ�
//			{
//				LED1=1;
//				duoji(0);
//			}
//			if(RecBuf[9]=='2')//RecBuf[9]�൱��������"+IPD��x,x:"��0��8��9���ַ�
//			{
//				duoji(90);
//			}
//			if(RecBuf[9]=='3')//RecBuf[9]�൱��������"+IPD��x,x:"��0��8��9���ַ�
//			{
//				duoji(145);
//			}
//			if(RecBuf[9]=='4')//RecBuf[9]�൱��������"+IPD��x,x:"��0��8��9���ַ�
//			{
//				LED2=1;
//				duoji(180);
//			}
//        }
//    _ur2f = 0;  //�����־
//
//}
