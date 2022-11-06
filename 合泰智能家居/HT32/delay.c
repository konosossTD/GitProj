#include "HT66F2390.h"
#include "delay.h"
void delay_ms(unsigned int t)
{
	do
	{
	GCC_DELAY(2000);
		}while(t--);
	}

	