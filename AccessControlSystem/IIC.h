#ifndef __IIC_H__ 
#define __IIC_H__ 


sbit SCL = P2^0;
sbit SDA = P2^1;
void IIC_start(void);
	
void IIC_writeByte(char temp);
	
void LCD_write_command(char comm);
	
void LCD_write_data(char data1);



#endif