#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include "HT66F2390.h"

//=========================================================================
//					Variable
//=========================================================================
static volatile unsigned char Show_words[16] __attribute__ ((at(0x1a0)));
//=========================================================================


//=========================================================================
//					I/O define
//=========================================================================
#define	DATA_PORT		_pc
#define	DATA_PORT_PU	_pcpu
#define	DATA_PORT_C		_pcc
#define DATA_PORT_D0	_pc0
#define DATA_PORT_D1	_pc1
#define DATA_PORT_D2	_pc2
#define DATA_PORT_D3	_pc3
#define DATA_PORT_D4	_pc4
#define DATA_PORT_D5	_pc5
#define DATA_PORT_D6	_pc6
#define DATA_PORT_D7	_pc7

#define PORT_RS		_pd0
#define PORT_RS_C	_pdc0
#define PORT_RW		_pd1
#define PORT_RW_C	_pdc1
#define PORT_E		_pd2
#define PORT_E_C	_pdc2
//=========================================================================


//=========================================================================
//					typedef
//=========================================================================
#define Screen_CLR		0x01
#define Curson_RET		0x02
#define Input_SET		0x06
#define Display_ON		0x0c
#define Display_OFF		0x08
#define FUNCTION_SET	0x38	//0x28
//=========================================================================

//=========================================================================
//					Function
//=========================================================================
void Display_Initial();
void Display_Word(u8 Display_LINE,u8 Display_CUR,u8 Display_DATA);
void Write_Operation(u8 Write_Code,u8 RS);
unsigned char Read_Operation(u8 RS);
//=========================================================================



#endif












