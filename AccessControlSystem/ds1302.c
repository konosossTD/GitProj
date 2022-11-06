#include "ds1302.h"
#include "intrins.h"



uchar time_buf[8] = {0x20,0x21,0x12,0x13,0x17,0x54,0x00,0x43};//初始时间
uchar realtime[14];//当前时间
uchar sec_buf=0;  //秒buf
uchar sec_flag=0; //秒标志位
uint time_buf_ten[8];
bit mute_flag=0;

void Clock_Init(void) 
{
	RST=0;			//RST脚置低
	SCK=0;			//SCK脚置低
}

void Clock_write(uchar addr, uchar d) 
{
	uchar i;
	RST=1;					//启动DS1302总线	
	//写入目标地址：addr
	addr = addr & 0xFE;   //最低位置零，寄存器0位为0时写，为1时读
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			DIO=1;
			}
		else {
			DIO=0;
			}
		SCK=1;      //产生时钟
		SCK=0;
		addr = addr >> 1;
		}	
	//写入数据：d
	for (i = 0; i < 8; i ++) {
		if (d & 0x01) {
			DIO=1;
			}
		else {
			DIO=0;
			}
		SCK=1;    //产生时钟
		SCK=0;
		d = d >> 1;
		}
	RST=0;		//停止总线
}


uchar Clock_read(uchar addr) {

	uchar i,temp;	
	RST=1;					//启动DS1302总线

	//写入目标地址：addr
	addr = addr | 0x01;    //最低位置高，寄存器0位为0时写，为1时读
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			DIO=1;
			}
		else {
			DIO=0;
			}

		SCK=1;

		SCK=0;

		addr = addr >> 1;
		}	
	//输出数据：temp
	for (i = 0; i < 8; i ++) {
		temp = temp >> 1;
		if (DIO) {
			temp |= 0x80;
			}
		else {
			temp &= 0x7F;
			}

		SCK=1;

		SCK=0;
		}	

	RST=0;					//停止DS1302总线
	return temp;
}

void Clock_write_time(void) 
{
	Clock_write(ds1302_control_add,0x00);			//关闭写保护 
	Clock_write(ds1302_sec_add,0x80);				//暂停时钟 
	Clock_write(ds1302_charger_add,0xa9);	    	//涓流充电 
	Clock_write(ds1302_year_add,time_buf[1]);		//年 
	Clock_write(ds1302_month_add,time_buf[2]);	//月 
	Clock_write(ds1302_date_add,time_buf[3]);		//日 
	Clock_write(ds1302_hr_add,time_buf[4]);		//时 
	Clock_write(ds1302_min_add,time_buf[5]);		//分
	Clock_write(ds1302_sec_add,time_buf[6]);		//秒
	Clock_write(ds1302_day_add,time_buf[7]);		//周 
	Clock_write(ds1302_control_add,0x80);			//打开写保护     
}

void Clock_read_time(void)  //读到buffer里
{

	time_buf[1]=Clock_read(ds1302_year_add);		//年十位和个位 s
	time_buf[2]=Clock_read(ds1302_month_add);		//月十位和个位
	time_buf[3]=Clock_read(ds1302_date_add);		//日十位和个位
	time_buf[4]=Clock_read(ds1302_hr_add);			//时十位和个位
	time_buf[5]=Clock_read(ds1302_min_add);			//分十位和个位
	time_buf[6]=(Clock_read(ds1302_sec_add))&0x7f;//秒十位和个位，屏蔽秒的第7位，避免超出59
	time_buf[7]=Clock_read(ds1302_day_add);		//周
	if(time_buf[4]>0x22 || time_buf[4]<0x07){mute_flag=1;}
	else{mute_flag=0;}
}

void separate()//
{
	uint i=0;
	Clock_read_time();  //读取时间 
	realtime[0]=(time_buf[0]>>4);   //年千位
	realtime[1]=(time_buf[0]&0x0F); //年百位 
	realtime[2]=(time_buf[1]>>4);   //年十位
	realtime[3]=(time_buf[1]&0x0F); //年个位 
	realtime[4]=(time_buf[2]>>4);   //月十位
	realtime[5]=(time_buf[2]&0x0F); //月个位 
	realtime[6]=(time_buf[3]>>4);   //日十位
	realtime[7]=(time_buf[3]&0x0F); //日个位 
	realtime[8]=(time_buf[4]>>4);   //小十位
	realtime[9]=(time_buf[4]&0x0F); //小个位 
	realtime[10]=(time_buf[5]>>4);   //分十位
	realtime[11]=(time_buf[5]&0x0F); //分个位
	realtime[12]=(time_buf[6]>>4);   //秒十位
	realtime[13]=(time_buf[6]&0x0F); //秒个位   
//	for(i=0;i<14;i++)
//	{	
//		Send_Byte(realtime[i]+'0');

//	
//	}
}


