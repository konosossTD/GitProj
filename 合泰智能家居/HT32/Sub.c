#include "Function.h"
#include "Display.h"
#include "UART.h"

const u8 Line0[16]="  1602 DISPLAY  ";		//default display line 0
const u8 Line1[16]="  Hello World!  ";		//default display line 1


void SYS_Initial()
{
//---------		HIRC change frequency		---------
	_hircc=0x09;			//run HIRC 16M
	while(_hircf==0);		//wait for the HIRC oscillator stable

//---------	Enable HXT & Fsys switch to HXT	---------
//	_pbs1=0xf0;				// OSC2	@ PB7, OSC1 @PB6
//	_hxtm=1;				//HXT frequency >10 MHz
//	_hxten=1;				//run Crystal 11.0592M
//	while(_hxtf==0);		//wait for the Crystal oscillator stable
//	_fhs=1;
//	_hircen=0;

		
	_wdtc=0b01010110;		//WDT time-out period  4S	
	//Clear Ram0 Data
	for(_mp0=0x7f;_mp0<0xff;)
	{
		_mp0++;
		_iar0=0;
	}	
	
	//Clear Ram1 Data		
	_mp1h=1;
	for(_mp1l=0x7f;_mp1l<0xff;)
	{
		_mp1l++;
		_iar1=0;
	}

	_mp1h=0;


}




void Show_1st_Message_to_PC()
{
	u8	i;
	for(i=0;i<16;i++)UART_Send_Byte(Line0[i]);
	GCC_DELAY(160000);	
	for(i=0;i<16;i++)UART_Send_Byte(Line1[i]);
}

void Show_1st_Message_to_1602()
{
	u8	cur;
	for(cur=0;cur<16;cur++)Display_Word(0,cur,Line0[cur]);
	for(cur=0;cur<16;cur++)Display_Word(1,cur,Line1[cur]);	
}

void Show_UART_Data_to_1602()
{
	u8	cur;
	for(cur=0;cur<16;cur++)Display_Word(0,cur,Line0[cur]);
	for(cur=0;cur<16;cur++)Display_Word(1,cur,20);
	GCC_DELAY(100000);
	for(cur=0;cur<RCV_Data_Length;cur++)Display_Word(1,cur,Show_words[cur]);
	
}



