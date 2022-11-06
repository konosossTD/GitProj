#ifndef __1602_h__ 
#define __1602_h__ 

#define uchar unsigned char
#define uint unsigned int

	

	
void Init_Lcd(void);
	
//void Write_LCD(int x, int y, char *str);
	
void LCD_write_word(unsigned char *s) ;
	
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);

void LCD_write_line(uint line,unsigned char *s);

void clear();

uchar mainpage();

void menu();

void settings();

void password();

void date_update(int i);


#endif