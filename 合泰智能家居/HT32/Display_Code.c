#include "Function.h"
#include "Display.h"

/*对1602读忙标志位*/
void Read_BF()			
{
	PORT_E=0;
	PORT_RS=0;
	PORT_RW=1;
	GCC_NOP();
	PORT_E=1;
	GCC_DELAY(5);
	while(DATA_PORT_D7==1);
	PORT_E=0;
}

/*写操作函数*/
//Write_Code为写入代码, RS=0写入命令, RS=1写入数据
void Write_Operation(u8 Write_Code,u8 RS)
{
	Read_BF();
	if(RS==0)
	{
		PORT_RS=0;
	}else PORT_RS=1;
	
	GCC_NOP();
	PORT_RW=0;
	DATA_PORT_C=0;
	DATA_PORT=Write_Code;
	
	GCC_NOP();//tSP1	
	PORT_E=1;		
	GCC_DELAY(10);//tPW
	PORT_E=0;
	
	GCC_DELAY(6);//tHD
	DATA_PORT_C=0xff;
	
}

/*读操作函数*/
unsigned char Read_Operation(u8 RS)
{
	u8 Read_code;	
	Read_BF();
	if(RS==0)
	{
		PORT_RS=0;
	}else PORT_RS=1;
		
	DATA_PORT_C=0xff;
	GCC_CLRWDT();
	PORT_RS=1;
	PORT_RW=1;	
	GCC_NOP();//tSP1
	PORT_E=1;
	GCC_DELAY(6);//tD	
	Read_code=DATA_PORT;
	PORT_E=0;
	return Read_code;
}

/*显示函数*/
//Display_LINE为显示第几行, Display_CUR为所在行的第几个字, Display_DATA为需要显示的数据
void Display_Word(u8 Display_LINE,u8 Display_CUR,u8 Display_DATA)
{
	if(Display_LINE)
	{
		Display_CUR|=0x40;
	}
	Display_CUR|=0x80;
	
	Write_Operation(Display_CUR,0);
	GCC_DELAY(100);
	Write_Operation(Display_DATA,1);
	GCC_DELAY(100);
}


/*1602初始化函数*/
void Display_Initial()
{
	PORT_RS_C=0;
	PORT_RW_C=0;
	PORT_E_C=0;
	DATA_PORT_PU=0xff;
	PORT_E=0;
	PORT_RS=1;
	PORT_RW=1;
	DATA_PORT_C=0xff;

	GCC_DELAY(40000);			//15mS
	Write_Operation(FUNCTION_SET,0);	
	GCC_DELAY(13000);			//5mS
//	Write_Operation(FUNCTION_SET,0);
//	GCC_DELAY(300);				//100uS
//	Write_Operation(FUNCTION_SET,0);
//	GCC_DELAY(100);
//	Write_Operation(FUNCTION_SET,0);
	GCC_DELAY(100);
	Write_Operation(Display_OFF,0);
	GCC_DELAY(100);
	Write_Operation(Screen_CLR,0);
	GCC_DELAY(6000);
	Write_Operation(Input_SET,0);
	GCC_DELAY(100);
	Write_Operation(Display_ON,0);
	GCC_DELAY(100);

}



