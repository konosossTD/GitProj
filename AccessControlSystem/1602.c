#include <STC12C5A60S2.H>
#include "intrins.h"
#include "delay.h"
#include "IIC.h"
#include "1602.h"
#include "ds1302.h"
#define uchar unsigned char
#define uint unsigned int 
extern login;
extern uchar realtime[14];
extern lastkey;
extern bit homepage;
//#define                        L1                0x80            // 第一行写入地址
//#define                        L2                0xc0            // 第二行写入地址
        



char ADDR = 0x4E;    // PCF8574  T  模块的地址码
//char ADDR = 0x7e;    // PCF8574   AT  模块的地址码



//******************************** IIC 串口开始 ********************************************

//转到IIC



//******************************** 1602写命令 ********************************************
void LCD_write_command(char comm)
{
        char tmp;
        IIC_start();          // 串口开始
        IIC_writeByte(ADDR);  // 先选PCF 8574T 的地址  （应该是相当于选中的意思吧）
        
        tmp = comm & 0xF0;    // 取高四位
        tmp |= 0x0C;         //保留高4位为指令的高四位，低四位为   RS = 0, RW = 0, EN = 1  
        IIC_writeByte(tmp);  //从串口送出
		delay1(1);//original:20
        tmp &= 0xFB;        //Make EN = 0
        IIC_writeByte(tmp); 
        
        tmp = (comm & 0x0F) << 4 ;  //将指令的低四位 送到高位置保存
        tmp |= 0x0C;        //RS = 0, RW = 0, EN = 1
        IIC_writeByte(tmp);
        delay1(1);
        tmp &= 0xFB; // Make EN = 0
        IIC_writeByte(tmp);
        
}
//******************************** 1602写数据 ********************************************


void LCD_write_data(char data1)
{
        char tmp;
        IIC_start();
        IIC_writeByte(ADDR);   // 先选PCF 8574T 的地址  （应该是相当于选中的意思吧）
        
        tmp = data1 & 0xF0;
        tmp |= 0x0D; //RS = 0, RW = 0, EN = 1
        IIC_writeByte(tmp);
//        delay1(1);.
		_nop_();
        tmp &= 0xFB; //Make EN = 0
        IIC_writeByte(tmp); 
        
        tmp = (data1 & 0x0F) << 4 ;
        tmp |= 0x0D; //RS = 0, RW = 0, EN = 1
        IIC_writeByte(tmp);
//        delay1(1);
		_nop_();
        tmp &= 0xFB ; // Make EN = 0
        IIC_writeByte(tmp);
}


//******************************** 1602初始化 ********************************************


void Init_Lcd(void)
{
        LCD_write_command(0x33); //将8位总线转为4位总线
        delay1(1) ;
        LCD_write_command(0x32); //
        delay1(1) ; 
        LCD_write_command(0x28); // 4位数据线，显示2行，5*7点阵字符  ！如果是0x38  则为8位数据线，显示2行，5*7点阵字符
        delay1(1) ; 
        LCD_write_command(0x0C); // 开显示，关闭光标，不闪烁
        delay1(1) ;  
        LCD_write_command(0x06); // 设定输入方式，增量不位移
        delay1(1) ; 
        LCD_write_command(0x01); // 清屏
        delay1(1) ;
}








//*************************************** 在指定位置显示字符串 *************************************


//void Write_LCD(int x, int y, char *str)
//{
//        char addr;
//        if( x < 0){x = 0;}
//        if(x > 15)
//        {x = 15;}
//        if(y<0)
//        {y = 0;}
//        if(y > 1)
//        {y = 1;} 
//				
//        addr = 0x80 + 0x40 * y + x;   // Move cursor  移动光标
//        LCD_write_command(addr);
//        while (*str) 
//        {
//                LCD_write_data(*str++); 
//        }
//}


//-------------------------------------------- 显示字符串的函数 ----------------------------------------------------


void LCD_write_word(unsigned char *s)                  //显示字符串的函数
{
        while(*s!='\0')
        {
                LCD_write_data(*s);
                s++;
        }
}




//********************************* 指定位置显示一个字符*******************************************


/*
void Print_Char (unsigned char line,unsigned char num,unsigned char date)
{
                LCD_write_command(line+num);
                LCD_write_data(date);
}


*/




//按指定位置显示一个字符(针对1602液晶)-用在温度显示


void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
Y &= 0x3;
//X &= 0x13;                 //限制X不能大于15(19)，Y不能大于1(3)
//if (Y==1) X |= 0x40;        //当要显示第二行时地址码+0x40;

if (Y==0) X |= 0x80;
if (Y==1) X |= 0xc0; 
if (Y==2) X |= 0x80+0x14;
if (Y==3) X |= 0xc0+0x14; 	

//X |= 0x80;               // 算出指令码
LCD_write_command(X);    //这里不检测忙信号，发送地址码
LCD_write_data(DData);
}




//******************************** 1602行选写命令 ********************************************
void LCD_write_line(uint line,unsigned char *s)	
{
	homepage=0;
	if(line==1){
		DisplayOneChar(0,0,*s);
		s++;
	}
	if(line==2){
		DisplayOneChar(0,1,*s);
		s++;
	}
	if(line==3){
		DisplayOneChar(0,2,*s);
		s++;
	}
	if(line==4){
		DisplayOneChar(0,3,*s);
		s++;
	}
	LCD_write_word(s);

}

void clear()
{
		homepage=0;
        LCD_write_command(0x01); // 清屏
}


uchar mainpage()
{
	homepage=1;
	LCD_write_line(1,"-------18--231------");
	date_update(0);
	LCD_write_line(3,"   Welcome Home!!   ");
	LCD_write_line(4,"                    ");

	

	
	return 1;
}

void menu()
{
	homepage=0;
	LCD_write_line(1,"--------EDIT--------");
	LCD_write_line(2,"       1.Code       ");
	LCD_write_line(3,"  2.Finger  3.Data  ");
	
	if(login>0){DisplayOneChar(1,0,'-');}
	if(login==1){LCD_write_word("ROOT");}
	if(login==2){LCD_write_word("NO.2");}
	if(login==3){LCD_write_word("NO.3");}
	if(login==4){LCD_write_word("NO.4");}
	
	
}

void settings()
{
	homepage=0;
	LCD_write_line(1,"--------EDIT--------");
	LCD_write_line(2,"                    ");
	switch(lastkey)
	{
		case 1:
			LCD_write_line(3," 1.CHANGE  2.DELETE ");
			break;
		case 2:
			LCD_write_line(3," 1. ADD    2.DELETE ");
			break;
		case 3:
			LCD_write_line(3,"1.DOWNLOAD 2.UPLOAD ");
			break;		
		
	
	}

}

void password()
{
	homepage=0;
	LCD_write_line(1,"-------LOGIN--------");
	LCD_write_line(2,"PassWord:******     ");
	LCD_write_line(3,"                    ");
	LCD_write_line(4,"                    ");
}


void date_update(int i)
{
	
	
	int j=0;
	uchar tmp[14];
	separate();
	while(i<18)
	{
		tmp[i]=realtime[i]+'0';
//		Send_Byte(i);	
		switch(i)
		{	
			case 0:DisplayOneChar(0+i,1,0x20);j++;break;
			case 1:DisplayOneChar(0+i,1,0x20);j++;break;
			case 6:DisplayOneChar(0+i,1,0x2D);j++;break;
			case 9:DisplayOneChar(0+i,1,0x2D);j++;break;
			case 12:DisplayOneChar(0+i,1,0x2D);j++;break;
			case 15:DisplayOneChar(0+i,1,0x3A);j++;break;
			default:
			{
				DisplayOneChar(i,1,tmp[i-j]);
				break;
			}		
		}
		i++;
		

	}		
		

	
	



}