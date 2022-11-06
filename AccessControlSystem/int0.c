#include <STC12C5A60S2.H>
#include "beep.h"

void EXINT0() interrupt 0
{
	bee(5);
}


void exint0_init()
{
	IT0=0;
	EX0=1;
	ET0=1;
	EA=1;
	
}