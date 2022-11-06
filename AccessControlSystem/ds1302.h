#ifndef __DS1302_H__ 
#define __DS1302_H__ 
#include <STC12C5A60S2.H>
#include "uart.h"
#define uchar unsigned char
#define uint unsigned int 
#define ds1302_sec_add			0x80		//��
#define ds1302_min_add			0x82		//��
#define ds1302_hr_add			0x84		//ʱ
#define ds1302_date_add			0x86		//��
#define ds1302_month_add		0x88		//��
#define ds1302_day_add			0x8a		//����ַ
#define ds1302_year_add			0x8c		//��
#define ds1302_control_add		0x8e		//����
#define ds1302_charger_add		0x90 		//���
#define ds1302_clkburst_add		0xbe		//
sbit  SCK= P3^2 ;  
sbit  DIO= P3^4 ;
sbit  RST= P3^3 ;

void Clock_Init(void);
void Clock_write(uchar addr, uchar d);
uchar Clock_read(uchar addr);	
void Clock_write_time(void);
void Clock_read_time(void);
void separate();



#endif