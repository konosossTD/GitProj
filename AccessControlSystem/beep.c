#include "delay.h"
#include <STC12C5A60S2.H>
#include "intrins.h"
#include "beep.h"
#include "ds1302.h"
#define uchar unsigned char
#define uint unsigned int 
extern bit mute_flag;
void bee(uint times)
{
	if(!mute_flag)
	{
	do{
		P23=!P23;
		delay50(5);
		P23=!P23;
		delay50(3);
		times--;
		}while(times != 0);
	}
}

void beep()
{
	if(!mute_flag)
	{
		P23=!P23;
		delay50(1);
		P23=!P23;
	}
}

void beeps(uint times)
{
	if(!mute_flag)
	{
		do{
		P23=!P23;
		delay50(1);
		P23=!P23;
		delay50(1);

		times--;
		}while(times != 0);
	}

}


