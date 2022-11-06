#ifndef _FUNCTION_H_
#define _FUNCTION_H_
#include "HT66F2390.h"

typedef	unsigned char	u8;	
typedef unsigned int	u16	;
typedef unsigned long	u32	;

void SYS_Initial();
void Show_1st_Message_to_PC();
void Show_1st_Message_to_1602();
void Show_UART_Data_to_1602();


#endif