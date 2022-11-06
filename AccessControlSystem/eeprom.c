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
* ���� : x24c02_init()
* ���� : 24c02��ʼ���ӳ���
* ���� : ��
* ��� : ��
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
* ���� : start(void)
* ���� : ����I2C����
* ���� : ��
* ��� : ��
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
* ���� : stop()
* ���� : ֹͣI2C����
* ���� : ��
* ��� : ��
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
* ���� : writex()
* ���� : дһ���ֽ�
* ���� : j����Ҫд���ֵ��
* ��� : ��
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
* ���� : readx()
* ���� : ��һ���ֽ�
* ���� : ��
* ��� : ������ֵ
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
* ���� : ack()
* ���� : I2C����ʱ��
* ���� : ��
* ��� : ��
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
* ���� : x24c02_read()
* ���� : ��24c02�ж���ֵ
* ���� : address(Ҫ�������ַ��ȡֵ��256���ֽڣ�һҳ8���ֽڣ�256/8 = 32ҳ���ֽڵ�ַ�ĸ�5λΪҳ��ַ����0��ʼ0--31����3λΪÿҳ�ĵ�Ԫ��ַ����0��ʼ0--7
* ��� : ��24c02�ж�����ֵ
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
* ���� : x24c02_write()
* ���� : ��24c02��д������
* ���� : address(��ַ�� �� info��ֵ��256���ֽڣ�һҳ8���ֽڣ�256/8 = 32ҳ���ֽڵ�ַ�ĸ�5λΪҳ��ַ����0��ʼ0--31����3λΪÿҳ�ĵ�Ԫ��ַ����0��ʼ0--7
* ��� : ��
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

