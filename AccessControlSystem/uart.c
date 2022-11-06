#include <STC12C5A60S2.H>
#include "delay.h"
#include "uart.h"
#define uchar unsigned char
#define uint unsigned int 








void Uart_Init(void)
{
//	SCON = 0x50; /* 0101_0000��8λ�ɱ䲨���ʣ�����żУ��λ */
//    TMOD = 0x21; /* 0011_0001�����ö�ʱ��1Ϊ8λ�Զ���װ������ */
//    TH1 = 0xFD; /* ���ö�ʱ��1�Զ���װ�� */
//    TL1 = 0xFD;

//    ES = 1; /* �������ж� */
//    EA = 1; /* �����ж� */
//    TR1 = 1; /* ����ʱ��1 */
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	BRT = 0xFA;		//���ö�ʱ����ֵ
	AUXR |= 0x01;		//����1ʹ�ö��������ʷ�����Ϊ�����ʷ�����
	AUXR |= 0x10;		//�������������ʷ�����
	
    ES = 0; /* �������ж� */
    EA = 1; /* �����ж� */
	
	

}


void Send_Byte(unsigned char c)//UART Send a byte
{

	SBUF = c;
 	while(!TI);		//��TIΪ0ʱ�ȴ�ti��1 ������Ϊ1 
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
	while(!RI);	 //������Ϊ1 
	
	buffer = SBUF;

	RI = 0;
	return (buffer);
}
