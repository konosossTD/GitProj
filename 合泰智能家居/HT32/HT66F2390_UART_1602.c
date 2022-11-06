//***************************************************************************
//	IDE3000 Version			V7.95											*
//	Holtek (R) Cross C Compiler Version V3.51								*
//	Version					1.1												*
//	Author					Hifern											*
//	Date					2018/01/18										*
//***************************************************************************
//	Voltage					5V												*
//	Frequence				HIRC 8/12 MHz/	Crystal 8/11.0592/12 MHz		*
//	UART mode				8-N-1	mode									*
//	High/Low Freq  OSC		HIRC + LIRC/XTAL + LIRC							*
//	Package					64 LQFP											*
//																			*
//	ICP/OCDS Program PIN													*
//	VDD						PIN 6	VDD										*
//	VSS						PIN 7	VSS										*
//	OCDSCK					PIN 16	PA2										*
//	OCDSDA					PIN 19	PA0										*
//	Data Transfer PIN														*
//	TX0						PIN 60	PD5										*
//	RX0						PIN 59	PD4										*
//	1602 Transfer PIN														*
//	D0						Pin 43	PC0										*
//	D1						Pin 44	PC1										*
//	D2						Pin 45	PC2										*
//	D3						Pin 46	PC3										*
//	D4						Pin 47	PC4										*
//	D5						Pin 48	PC5										*
//	D6						Pin 49	PC6										*
//	D7						Pin 50	PC7										*
//	RS						Pin 51	PD0										*
//	RW						Pin 52	PD1										*
//	E						Pin 53	PD2										*
//	BLA						VDD												*
//	BLK						VSS												*
//***************************************************************************

#include "Function.h"
#include "Display.h"
#include "UART.h"


void delay_ms(unsigned int t)
{
	do
	{
	GCC_DELAY(2000);
		}while(t--);
	}
	
void main()
{
	u8	i;
	UART_SET();	
	while(1)
	{
	
	UART_Send_Byte(0xef);
	delay_ms(1000);		
		/*GCC_CLRWDT();*/
	}

}



