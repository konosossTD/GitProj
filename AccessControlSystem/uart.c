#include <STC12C5A60S2.H>
#include "delay.h"
#include "uart.h"
#define uchar unsigned char
#define uint unsigned int 








void Uart_Init(void)
{
//	SCON = 0x50; /* 0101_0000，8位可变波特率，无奇偶校验位 */
//    TMOD = 0x21; /* 0011_0001，设置定时器1为8位自动重装计数器 */
//    TH1 = 0xFD; /* 设置定时器1自动重装数 */
//    TL1 = 0xFD;

//    ES = 1; /* 允许串口中断 */
//    EA = 1; /* 开总中断 */
//    TR1 = 1; /* 开定时器1 */
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器时钟1T模式
	BRT = 0xFA;		//设置定时重载值
	AUXR |= 0x01;		//串口1使用独立波特率发射器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发射器
	
    ES = 0; /* 允许串口中断 */
    EA = 1; /* 开总中断 */
	
	

}


void Send_Byte(unsigned char c)//UART Send a byte
{

	SBUF = c;
 	while(!TI);		//当TI为0时等待ti置1 发送完为1 
	TI = 0;

}

void Send_Str(uchar *str)
{

while(*str != '\0')
{
	Send_Byte(*str);
	str++;
}
}




uchar Uart_Receive_Byte()//UART Receive a byteg
{	
	uchar buffer;
	while(!RI);	 //接收完为1 
	
	buffer = SBUF;

	RI = 0;
	return (buffer);
}
