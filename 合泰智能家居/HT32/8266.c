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

     _brg2 = 207;  //波特率 2400
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
    while(_ridle2 == 0); //等待接收完成
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
	GCC_DELAY(1000);
}

void ESP8266_ModeInit(void)
{
	ESP8266_SendCmd("AT+RST");
	delay_ms(1000);
	ESP8266_SendCmd("AT+CWMODE=1");//设置路由器模式
	delay_ms(1000);
	ESP8266_SendCmd("AT+CWJAP=\"zjj\",\"12345678\""); 
	delay_ms(10000);
//	ESP8266_SendCmd("AT+RST");//重新启动wifi模块（可省略）
	ESP8266_SendCmd("AT+CIPMUX=1");	//开启多连接模式，允许多个各客户端接入
	delay_ms(1000);
	ESP8266_SendCmd("AT+CIPSERVER=1,8899");	//启动TCP/IP 端口为8080 
	delay_ms(1000);	
}
//DEFINE_ISR(UART2,0x3C)
//{		
//		char RecBuf[50];
//		int data_count=0;
//		int buff_length;
//        buff_length=re_buff(RecBuf,'\n')+48;  //长度大于9会显示乱码的长度值，原因就是ASCII转换
//        if(data_count==0) 
//        { 
//        	if(RecBuf[9]=='1')//RecBuf[9]相当于跳过了"+IPD，x,x:"这0到8这9个字符
//			{
//				LED1=1;
//				duoji(0);
//			}
//			if(RecBuf[9]=='2')//RecBuf[9]相当于跳过了"+IPD，x,x:"这0到8这9个字符
//			{
//				duoji(90);
//			}
//			if(RecBuf[9]=='3')//RecBuf[9]相当于跳过了"+IPD，x,x:"这0到8这9个字符
//			{
//				duoji(145);
//			}
//			if(RecBuf[9]=='4')//RecBuf[9]相当于跳过了"+IPD，x,x:"这0到8这9个字符
//			{
//				LED2=1;
//				duoji(180);
//			}
//        }
//    _ur2f = 0;  //清除标志
//
//}
