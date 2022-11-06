#include "HT66F2390.h"
#include "DHT11.h"
#include "delay.h"
#include "UART.h"
#include "MQ.h"
#include "ESP.h" 
#define Data _pa4
#define Datac _pac4


u8 R_H,R_L,T_H,T_L,RH,RL,TH,TL,revise,Temp=0xEE,Humid=0xEE,rec_dat[17],i;
u32 time2_count=0,a;
//Time Base 0 时基中断函数
//void __attribute((interrupt(0x24))) ISR_TB0(void)
//{
//	//time base	1秒一次定时中断	
//	time2_count++;
//	if(time2_count==10)
//		{
//			DHT11_receive();           
//		}
//		if(time2_count==15)
//		{
//			time2_count=0;
//			for(a=0;a<17;a++)
//			{
//	   		send_char(rec_dat[a]);
//			}
//			MQ_2();            
//		}
//
//}
//MF0 多功能中断函数
//void __attribute((interrupt(0x38))) ISR_MF0(void)
//{
//	//多功能中断0	10ms秒STM0产生一次定时中断
//	if(_stm2af)
//	{
//		time2_count++;
//		if(time2_count==10)
//		{
//			time2_count=0;
//			for(i=0;a<17;a++)
//			{
//	   		send_char(rec_dat[a]);
//			}
//			/*MQ_2(); */           
//		}
//		_stm2af=0;
//	}
//}
void DHT11_time_init()
{
	_psc0r=	0x02;			//fsub
	_tb0c=	0b00000101;		// 2^15		1秒产生一次中断
	_tb0on=1;
	_tb0f=0;
	_tb0e=1;
	_emi=1;
}	
void DHT11_Rst()
{
  Datac=0;
   Data=1;
  GCC_DELAY(4);
   Data=0;
   delay_ms(25);   //延时18ms以上
   Data=1;
   GCC_DELAY(60);
}
u8 DHT11_rec_byte()      //接收一个字节
{
   u8 i,dat=0;
  for(i=0;i<8;i++)    //从高到低依次接收8位数据
   {  while(!Data);   /*等待50us低电平过去*/
     GCC_DELAY(70);     //延时60us，如果还为高则数据为1，否则为0 
      dat<<=1;           //移位使正确接收8位数据，数据为0时直接移位
      if(Data==1)    //数据为1时，使dat加1来接收数据1
         dat+=1;
      while(Data);  //等待数据线拉低
      
   }
    return dat;
    	
}

void DHT11_receive()      //接收40位的数据
{

   DHT11_Rst();
   Datac=1;
    if(Data==0)
    {    
    	while(Data==0);    	
    	GCC_DELAY(160);
        R_H=DHT11_rec_byte();    //接收湿度高八位  
        R_L=DHT11_rec_byte();    //接收湿度低八位 
        T_H=DHT11_rec_byte();    //接收温度高八位  
        T_L=DHT11_rec_byte();    //接收温度低八位   
        revise=DHT11_rec_byte(); //接收校正位
       	GCC_DELAY(50);     //结束
        if((R_H+R_L+T_H+T_L)==revise)      //校正
        {
            RH=R_H;
            RL=R_L;
            TH=T_H;
            TL=T_L;
        } 
	    rec_dat[0]='0'+(RH/10);
        rec_dat[1]='0'+(RH%10);
		rec_dat[2]=' ';	
		rec_dat[3]='R';
		rec_dat[4]='H';
        rec_dat[5]=' ';
        rec_dat[6]=' ';
        rec_dat[7]=' ';
        rec_dat[8]=' ';
        rec_dat[9]='0'+(TH/10);
        rec_dat[10]='0'+(TH%10);
		rec_dat[11]=' ';
		rec_dat[12]= 'C';
		rec_dat[13]='\r';
        rec_dat[14]='\n';
        rec_dat[15]=' ';
        rec_dat[16]=' ';
       
    }
}