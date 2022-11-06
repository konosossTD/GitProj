#include <STC12C5A60S2.H>
#include "servo.h"
#include "beep.h"
#define uchar unsigned char
#define uint unsigned int 
	

extern servo_val;

void set_servo_value(uint val)
{
	if((val>=0)&&(val<=25))
	{
		servo_val=val;
	}
	else
		bee(1);
}