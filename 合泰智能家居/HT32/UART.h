#ifndef _UART_H_
#define _UART_H_
#include "HT66F2390.h"

//=========================================================================
//					Variable
//=========================================================================
extern volatile  unsigned char Rcv_Time_Counter,Data_Counter,RCV_Data_Length;
extern volatile  bit RECEIVE_Finish_Flag;
static volatile unsigned char Rcv_Data[16] __attribute__ ((at(0x180)));
//=========================================================================

//=========================================================================
//					I/O define
//=========================================================================

//=========================================================================
//					Function
//=========================================================================
void SYS_Initial();
void __attribute((interrupt(0x3c))) ISR_UART_Receive(void);
void __attribute((interrupt(0x24))) ISR_TB0(void);
void UART_Send_Byte(char s);
void UART_SET();
void UART_Send_buff(char *s);
/*int re_buff0(char *rbuff,char c);*/
//=========================================================================




//=========================================================================
//					User configuration
//=========================================================================
#define FSYS_FQC 					16000000		//Fsys Frequence	
#define Baud_Rate_0 				115200			//Baud Rate	
#define BRGH_0						1				//High speed
#define Wait_CLR_Count				2				//Receive wait time(1/2s)

//	BRGH=0
//FSYS_FQC			Baud Rate
//8000000			600,1200,2400,4800,9600,19200
//11059200			1200,2400,4800,9600,14400,19200,57600
//12000000			1200,2400,4800,9600,14400,19200,38400
//16000000			1200,2400,4800,9600,14400,19200

//	BRGH=1
//FSYS_FQC			Baud Rate
//8000000			2400,4800,9600,14400,19200,38400
//11059200			4800,9600,14400,19200,38400,57600,115200,230400
//12000000			4800,9600,14400,19200,38400,57600
//16000000			4800,9600,14400,19200,38400,57600,115200
//=========================================================================

#if	BRGH_0	
//BR=FH/(16*(N+1))	calculate N with half adjust
#define SET_BRG_0 	(FSYS_FQC/Baud_Rate_0/8+1)/2-1	
#else
//BR=FH/(64*(N+1))	calculate N with half adjust
#define SET_BRG_0 	(FSYS_FQC/Baud_Rate_0/32+1)/2-1	
#endif


#endif







