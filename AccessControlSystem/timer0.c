#include <STC12C5A60S2.H>
#include "servo.h"
#include "beep.h"
#include "timer0.h"
#include "1602.h"
#define uchar unsigned char
#define uint unsigned int 
	
uint servo_period=0,servo_val=0;second_val=0,minute_val=0;
extern bit homepage;
bit time_flag=0;

void Timer0Init(void)		//100微秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;	//16noreload
	TL0 = 0xAE;		//设置定时初始值
	TH0 = 0xFB;		//设置定时初始值
	TF0 = 0;		//清除TF0标志.
	ET0=1;
	TR0 = 1;		//定时器0开始计时
	EA=1;
}




void timer0(void) interrupt 1 using 1
{

	if(second_val==10000){
		minute_val++;
		second_val=0;



	}
	if(minute_val==55)
	{
		
		minute_val=0;
		time_flag=1;  //1:up to date

	}

	second_val++;
	servo_period++;
	if(servo_period==200)
	{
		servo_period=0;
	}
	if(servo_period<=servo_val)
	{
		
		ServoIO =1;
	}
	else if(servo_period>servo_val)
	{

		ServoIO =0;
	}

	
	TL0 = 0xAE;		//设置定时初始值
	TH0 = 0xFB;		//设置定时初始值



}