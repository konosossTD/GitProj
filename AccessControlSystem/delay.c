#include <STC12C5A60S2.H>
#include "intrins.h"
#include "delay.h"
#define uchar unsigned char
#define uint unsigned int 

	


void delay1(uint t)		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 11*t;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}
void delay50(uint t)		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 3*t;
	j = 26;
	k = 223;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void delay100(uint t)		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 5*t;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void delay300(uint t)		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 13*t;
	j = 156;
	k = 83;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

