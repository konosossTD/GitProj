#ifndef _UART_H_
#define _UART_H_

#define uchar unsigned char
#define uint unsigned int

void Uart_Init(void);

void Send_Byte(unsigned char c);

void Send_Str(uchar *str);

uchar Uart_Receive_Byte();//UART Receive a byteg
#endif
