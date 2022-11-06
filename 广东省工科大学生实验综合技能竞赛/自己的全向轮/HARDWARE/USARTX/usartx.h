#ifndef __USRATX_H
#define __USRATX_H 
#include "sys.h"
extern unsigned char DM_Action0[5];
extern unsigned char DM_Action1[5];
extern unsigned char DM_Action2[5];
extern unsigned char DM_Action3[5];
extern unsigned char DM_Action4[5];
extern unsigned char DM_Action5[5];
extern int speed_x,speed_y,speed_z;
void usart3_send(u8 data);
void uart3_init(u32 pclk2,u32 bound);
int USART3_IRQHandler(void);
void UART3_Put_StringL(unsigned char *Str,unsigned char len);
void UART_DM_ReportData(unsigned char data[]);
void UART2_DM_ReportData(unsigned char data[]);
#endif

