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

void Timer0Init(void)		//100΢��@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;	//16noreload
	TL0 = 0xAE;		//���ö�ʱ��ʼֵ
	TH0 = 0xFB;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־.
	ET0=1;
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
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

	
	TL0 = 0xAE;		//���ö�ʱ��ʼֵ
	TH0 = 0xFB;		//���ö�ʱ��ʼֵ



}