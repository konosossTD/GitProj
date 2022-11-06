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
char RecBuf2[50],door;//u8表示char  u16表示int
u8 RecBuf0[50];
void esp8266_init()
{
	_pbs0 = _pbs0 | 0b11101100;//pb1:tx,pb2:rx,pb3;ptp2
	_u2cr1 = 0b10000000;
    _u2cr2 = 0b11101100;
    _ur2e = 1;

     _brg2 = 51;  //波特率 9600
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
	char rdata;//数据长度
    while(_ridle2 == 0); //等待接收完成
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
	int data_count0=0;    //数据长度
    while(_ridle0 == 0); //等待接收完成
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
	while(*pbuf!='\0') //遇到空格跳出循环	
	{
		send_char(*pbuf);
		GCC_DELAY(10);//延迟5us,自己弄一下函数模块
		pbuf++;	
	}
	GCC_DELAY(100);
	send_char('\r');//回车
	GCC_DELAY(100);
	send_char('\n');//换行
	delay_ms(1000);
}


void ESP8266_ModeInit(void)
{
	ESP8266_SendCmd("AT+RST");
	delay_ms(1000);
	ESP8266_SendCmd("AT+CWMODE=1");//设置路由器模式
	delay_ms(1000);
	ESP8266_SendCmd("AT+CWJAP=\"jj\",\"12345678\""); 
	delay_ms(10000);
//	ESP8266_SendCmd("AT+RST");//重新启动wifi模块（可省略）
	ESP8266_SendCmd("AT+CIPMUX=0");	//开启单连接模式
	delay_ms(1000);
	ESP8266_SendCmd("AT+CIPSTART=\"TCP\",\"192.168.69.125\",8888");	//连接服务器
	delay_ms(1000);
	ESP8266_SendCmd("AT+CIPMODE=1");	//透传模式
	delay_ms(1000);	
	ESP8266_SendCmd("AT+CIPSEND");	//数据传输模式
	delay_ms(1000);
}

DEFINE_ISR(UART0,0x3C)
{	
		LED1C=0;  //設定LED1為輸出模式
		LED2C=0;  //設定LED2為輸出模式
		_pgc2=0;
		_pgc4=0;
		int buff_length0,buff_length2;
        buff_length2=re_buff(RecBuf2,'\n')+48;  //长度大于9会显示乱码的长度值，原因就是ASCII转换
       /* buff_length0=re_buff0(RecBuf0,'\n')+48;*/
//        if(data_count0==0) 
//        {
//        	UART_Send_Byte(RecBuf0);
//        }
		if(_ur0f==1) //HT66收到指纹锁开门请求，发动舵机并且发送给手机端 open door
		{
			ESP8266_SendCmd("The door was opened");
			duoji1(0);
		}
        if(_ur2f==1)//1:远程打开门。2：开窗户。3：开灯 
        { 
        	if(RecBuf2[u]=='1')//RecBuf[9]相当于跳过了"+IPD，x,x:"这0到8这9个字符
			{
				LED1=1;
				send_char('1');
				duoji0(45);
				duoji1(20);
				duoji2(20);
				_pg2=1;
				_pg4=1;
			}
			if(RecBuf2[u]=='2')//RecBuf[9]相当于跳过了"+IPD，x,x:"这0到8这9个字符
			{	
				LED1=0;
				send_char('2');
				duoji0(90);
				duoji1(90);
				duoji2(90);
			}
			if(RecBuf2[u]=='3')//RecBuf[9]相当于跳过了"+IPD，x,x:"这0到8这9个字符
			{
				LED2=0;
				send_char('3');
				duoji0(120);
				duoji1(120);
				duoji2(120);
			}
			if(RecBuf2[u]=='4')//RecBuf[9]相当于跳过了"+IPD，x,x:"这0到8这9个字符
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
    _ur2f = 0;  //清除标志
    _ur0f = 0;  //清除标志
//    _mf5f=0;
//    _emi=0;

}
