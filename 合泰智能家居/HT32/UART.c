#include "UART.h"
#include "ESP.h"
//int data_count0=0;    //数据长度
//int len0;
volatile  unsigned char Rcv_Time_Counter,Data_Counter,RCV_Data_Length;
volatile  bit RECEIVE_Finish_Flag;
void UART_SET()
{
//	_pas1=0xf0;				//TX0 @ Pa7, RX0 @ Pa6
//	_ifs3=0x01;				
//	_u0cr1=0b10000000;		//Enable UART0
//#if	BRGH_0
//	_u0cr2=0b11101100;
//#else
//	_u0cr2=0b11101100;
//#endif	
//	_brg0=51;
//	RECEIVE_Finish_Flag=0;
//	_wdtc=0b10101110;		//WDT time-out period  4S
//	_psc0r=0x02;			//Time base clock source choose Fsub
//	_tb0c=0b10110101;		//time base interrupt 1/4 S
//	
//	
///*	_tb0f=0;*/
///*	_ur0f=0;*/
//	_mf5f=0;
///*	_tb0e=1;*/
//	_ur0e=1;
//	_mf5e=1;	
//	_emi=1;
	_pas1=0xf0;				//TX0 @ Pa7, RX0 @ Pa6
	_u0cr1 = 0b10000000;
    _u0cr2 = 0b11101100;
    _ur0e = 1;

     _brg0 = 103;  //波特率 4800
    _mf5e = 1;
    _emi =1;
	
}


//Send	one byte
void UART_Send_Byte(char s)
{	
/*	while(_u0sr==0);*/
    _txr_rxr0 =s;
    while(_tidle0==0);
}
void UART_Send_buff(char *s)
{
    while(*s)
    {
        UART_Send_Byte(*s);
        s++;
    }    
}
//int re_buff0(char *rbuff,char c)
//{ char rdata;
//    while(_ridle0 == 0); //等待接收完成
//    rdata =_txr_rxr0;
//    if(rdata!=c)
//    {
//        rbuff[data_count0]=rdata;
//        data_count0++;
//    }     
//     else
//     {
//         rbuff[data_count0]='\0';
//         len0=data_count0;
//            data_count0=0; 
//            return len0;
//     }
//    return data_count0;
//}
//DEFINE_ISR(UART0,0x3C)
//{
//    char buff[100];
//
//    int buff_length;
//        buff_length=re_buff(buff,'\n')+48;  //长度大于9会显示乱码的长度值，原因就是ASCII转换
////        if(data_count==0) 
////        {
////        send_buff(buff);  
////       	send_char( buff_length);
////        }
//    _ur0f = 0;  //清除标志
//}




