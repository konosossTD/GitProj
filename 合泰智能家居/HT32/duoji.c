#include "HT66F2390.h"
#include "DUOJI.h"
#include "delay.h"
void duoji_start0()
{
	_ptm0c0=	0b01010000;			//时钟源Fsvb
	_ptm0c1=	0b10101000;			//PWM模式, PWM输出, 初始高, 比较器P匹配做为周期
	_ptm0rpl=0x38;          // 周期568 频率50hz
    _ptm0rph=0x02;
	_pcs0=0x20; //ptp0
	_pcc2=0;	//PTP0输出
	_pt0on=1;
	_mf0f=0;
	_mf0e=1;
	_emi=1;
}

void duoji_start1()
{
	_ptm1c0=	0b01010000;			//时钟源Fsvb
	_ptm1c1=	0b10101000;			//PWM模式, PWM输出, 初始高, 比较器P匹配做为周期
    _ptm1rpl=0x38;          // 周期568 频率50hz
    _ptm1rph=0x02;
	_pes0=0x80;//pe3:ptp1
	_pec3=0;
	_pt1on=1;
	_mf1f=0;
	_mf1e=1;
	_emi=1;
}
void duoji_start2()
{
	_ptm2c0=	0b01010000;			//时钟源Fsvb
	_ptm2c1=	0b10101000;			//PWM模式, PWM输出, 初始高, 比较器P匹配做为周期
    _ptm2rpl=0x38;          // 周期568 频率50hz
    _ptm2rph=0x02;
	_pbc3=0;
	_pt2on=1;
	_mf2f=0;
	_mf2e=1;
	_emi=1;
}
void duoji0(u8 i)
{	
	if(i==0)
	{
	_ptm0al=0x10;
	_ptm0ah=0x00;
	}
	if(i==20)
	{
	_ptm0al=0x1a;
	_ptm0ah=0x00;
	}
	if(i==45)
	{
	_ptm0al=0x1c;
	_ptm0ah=0x00;
	}
	if(i==90)
	{
	_ptm0al=0x2a;
	_ptm0ah=0x00;
	}
	if(i==120)
	{
	_ptm0al=0x37;
	_ptm0ah=0x00;
	}
	if(i==135)
	{
	_ptm0al=0x3c;
	_ptm0ah=0x00;
	}
	if(i==180)
	{
	_ptm0al=0x49;
	_ptm0ah=0x00;
	}
}
void duoji1(u8 i)
{	
	if(i==0)
	{
	_ptm1al=0x10;
	_ptm1ah=0x00;
	}
	if(i==20)
	{
	_ptm1al=0x1a;
	_ptm1ah=0x00;
	}
	if(i==45)
	{
	_ptm1al=0x1c;
	_ptm1ah=0x00;
	}
	if(i==90)
	{
	_ptm1al=0x28;
	_ptm1ah=0x00;
	}
	if(i==120)
	{
	_ptm1al=0x37;
	_ptm1ah=0x00;
	}
	if(i==135)
	{
	_ptm1al=0x3c;
	_ptm1ah=0x00;
	}
	if(i==180)
	{
	_ptm1al=0x45;
	_ptm1ah=0x00;
	}
}
void duoji2(u8 i)
{	
	if(i==0)
	{
	_ptm2al=0x10;
	_ptm2ah=0x00;
	}
	if(i==20)
	{
	_ptm2al=0x1a;
	_ptm2ah=0x00;
	}
	if(i==45)
	{
	_ptm2al=0x1c;
	_ptm2ah=0x00;
	}
	if(i==90)
	{
	_ptm2al=0x2a;
	_ptm2ah=0x00;
	}
	if(i==120)
	{
	_ptm2al=0x37;
	_ptm2ah=0x00;
	}
	if(i==135)
	{
	_ptm2al=0x3c;
	_ptm2ah=0x00;
	}
	if(i==180)
	{
	_ptm2al=0x49;
	_ptm2ah=0x00;
	}
}