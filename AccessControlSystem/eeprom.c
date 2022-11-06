#include <STC12C5A60S2.H>
#include "intrins.h"
#include "eeprom.h"
#include "IIC.h"
#include "delay.h"
#include "uart.h"
#define uchar unsigned char
#define uint unsigned int
extern root[6];
extern usercode[3][8];
	
void flash(void) 
{

	unsigned char i, j;

	i = 8;
	j = 93;
	do
	{
		while (--j);
	} while (--i);

}
void wait(void)
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}
/********************************************************************
* 名称 : x24c02_init()
* 功能 : 24c02初始化子程序
* 输入 : 无
* 输出 : 无
***********************************************************************/
void eeprom_init(void) 
{	wp=0;
	wait();
	scl = 1;
	wait();
	sda = 1;
	wait();
}

/********************************************************************
* 名称 : start(void)
* 功能 : 启动I2C总线
* 输入 : 无
* 输出 : 无
***********************************************************************/
void start(void)
{
	scl = 1; 
	flash();
	sda = 1;
	flash(); 
	sda = 0; 
	flash(); 
	scl = 0; 
	flash();
}

/********************************************************************
* 名称 : stop()
* 功能 : 停止I2C总线
* 输入 : 无
* 输出 : 无
***********************************************************************/
void stop() 
{
	scl = 0;
	wait();
	sda = 0; 
	wait();
	scl = 1;
	wait();
	sda = 1;
	wait();
}

/********************************************************************
* 名称 : writex()
* 功能 : 写一个字节
* 输入 : j（需要写入的值）
* 输出 : 无
***********************************************************************/
void writebyte(uchar j)
{  
	uchar i,temp;
	temp = j;
	for(i=0; i<8; i++)
	{
		scl = 0; 
		wait(); 
		sda = (bit)(temp & 0x80); 
		wait();
		scl = 1; 
		wait();
		temp = temp << 1; 
	}
	scl = 0;
	flash(); 
}

/********************************************************************
* 名称 : readx()
* 功能 : 读一个字节
* 输入 : 无
* 输出 : 读出的值
***********************************************************************/
uchar readbyte(void)
{
	uchar i, j, k = 0;
	for(i=0; i<8; i++)
	{
		scl = 0;
//		flash();	
		_nop_();
		if(sda == 1)
		{
			j = 1;
		}
		else j = 0;
		k = (k << 1) | j; 
		scl = 1;
		_nop_();
//		flash();
	} 
	return(k);
}

/********************************************************************
* 名称 : ack()
* 功能 : I2C总线时钟
* 输入 : 无
* 输出 : 无
***********************************************************************/
void ack(void)
{
	uchar i = 0;
	scl = 1;
	_nop_();
	while((sda == 1) && (i < 255)) 
	{
		i++;
	}
	scl = 0;
	_nop_();
}

/********************************************************************
* 名称 : x24c02_read()
* 功能 : 从24c02中读出值
* 输入 : address(要在这个地址读取值）256个字节，一页8个字节，256/8 = 32页，字节地址的高5位为页地址，从0开始0--31，低3位为每页的单元地址，从0开始0--7
* 输出 : 从24c02中读出的值
***********************************************************************/
uchar eeprom_read(uchar address)
{
	uchar i;
	start();
	writebyte(0xa0);
	ack();
	writebyte(address);
	ack();
	start();
	writebyte(0xa1);
	ack();
	i = readbyte();
	stop();
	return(i);
}


/********************************************************************
* 名称 : x24c02_write()
* 功能 : 想24c02中写入数据
* 输入 : address(地址） ， info（值）256个字节，一页8个字节，256/8 = 32页，字节地址的高5位为页地址，从0开始0--31，低3位为每页的单元地址，从0开始0--7
* 输出 : 无
***********************************************************************/
void eeprom_write(uchar address, uchar info)
{
	start();
	writebyte(0xa0);
	ack();
	writebyte(address);
	ack();
	writebyte(info);
	ack();
	stop();

}


void upload()
{
	int i,j;
	uchar add=0x00;
	for(i=0;i<6;i++)
	{
		eeprom_write(add,root[i]);
		Send_Byte(root[i]);
		add=add+0x01;
	}
	add=0x00;
	for(i=0;i<3;i++)
	{	
		add&=0xF0;
		add=add+0x10;
		for(j=0;j<8;j++)
		{
			eeprom_write(add,usercode[i][j]);
			Send_Byte(usercode[i][j]);
			add=add+0x01;
		}
	}

}

void download()
{
	int i,j;
	uchar add=0x00;
	for(i=0;i<6;i++)
	{
		root[i]=eeprom_read(add);
		Send_Byte(root[i]);
		add=add+0x01;
		
	}
	add=0x00;
	for(i=0;i<3;i++)
	{
		add&=0xF0;
		add=add+0x10;
		for(j=0;j<8;j++)
		{
			usercode[i][j]=eeprom_read(add);
			Send_Byte(usercode[i][j]);
			add=add+0x01;
		}
	}

}

