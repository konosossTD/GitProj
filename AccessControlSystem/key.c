#include <STC12C5A60S2.H>
#include "beep.h"
#include "delay.h"
#include "intrins.h"
#include "uart.h"

#include "1602.h"
#include "BioSensor.h"
#include "timer0.h"
#define uchar unsigned char
#define uint unsigned int
	

extern short lastkey;
extern login;
extern bit time_flag;
extern bit homepage;
sbit SCL=P2^6;
sbit SDA=P2^7;



uint Keyboard_Scan() 
	{
    uchar key=0;
    uchar i;
    uint temp=0x0000;
    SDA=0;
    delay1(10);
    SDA=1;
    delay1(10);
		for(i=0;i<16;i++) 
		{
			SCL=0;
			_nop_();
			_nop_();
			SCL=1;
			_nop_();
			_nop_();
			if(SDA==0) {
				temp|=(1<<i);
			}   
		}
		for (i=0;i<16;i++) 
		{
			if ((temp & (1<<i)) > 0) {key=i+1;}
		}
		

		
		switch (key){
			case 0x02:beep();lastkey=1;return 1;
			case 0x03:beep();lastkey=2;return 2;
			case 0x04:beep();lastkey=3;return 3;
			case 0x05:beep();lastkey=4;return 4;
		
			case 0x06:beep();lastkey=5;return 5;
			case 0x07:beep();lastkey=6;return 6;
			case 0x08:beep();lastkey=7;return 7;
			case 0x09:beep();lastkey=8;return 8;
		
			case 0x0A:beep();lastkey=9;return 9;
			case 0x0B:beep();lastkey=0;return 0;
			case 0x0C:beep();lastkey=11;return 11; 
			case 0x0D:beep();lastkey=12;return 12; 
		
			case 0x0E:beep();lastkey=13;return 13;	
			case 0x0F:beep();lastkey=14;return 14;	
			case 0x10:beep();lastkey=15;return 15;	
			case 0x11:beep();lastkey=15;return 15;	
			Default:{lastkey=-1;	return -1;}
			
		
		 }

//def 							11&12:backspace  
//									13&14:quit  
//									15&16：enter
        
}
	
short waitkey(bit A)	//77按下  0~15键值  77手指  1：open
{

	lastkey=-1;
	
	while(1)
	{
		if(A && (finger_detect()==77))//返回77代表摁下
		{

			BIO_Find_Fingerprint();

//			
//			beeps(2);
//			date_update(0);
//			mainpage();
			lastkey=-1;
			return 77;
		}
		if(time_flag==1 && homepage==1 )
		{
			
			date_update(0);
			time_flag=0;

		}
		Keyboard_Scan();
		if(lastkey!=-1)
		{
			
			
			return lastkey;
	
		}

	}
	
}
