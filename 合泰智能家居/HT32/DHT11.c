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
//Time Base 0 ʱ���жϺ���
//void __attribute((interrupt(0x24))) ISR_TB0(void)
//{
//	//time base	1��һ�ζ�ʱ�ж�	
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
//MF0 �๦���жϺ���
//void __attribute((interrupt(0x38))) ISR_MF0(void)
//{
//	//�๦���ж�0	10ms��STM0����һ�ζ�ʱ�ж�
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
	_tb0c=	0b00000101;		// 2^15		1�����һ���ж�
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
   delay_ms(25);   //��ʱ18ms����
   Data=1;
   GCC_DELAY(60);
}
u8 DHT11_rec_byte()      //����һ���ֽ�
{
   u8 i,dat=0;
  for(i=0;i<8;i++)    //�Ӹߵ������ν���8λ����
   {  while(!Data);   /*�ȴ�50us�͵�ƽ��ȥ*/
     GCC_DELAY(70);     //��ʱ60us�������Ϊ��������Ϊ1������Ϊ0 
      dat<<=1;           //��λʹ��ȷ����8λ���ݣ�����Ϊ0ʱֱ����λ
      if(Data==1)    //����Ϊ1ʱ��ʹdat��1����������1
         dat+=1;
      while(Data);  //�ȴ�����������
      
   }
    return dat;
    	
}

void DHT11_receive()      //����40λ������
{

   DHT11_Rst();
   Datac=1;
    if(Data==0)
    {    
    	while(Data==0);    	
    	GCC_DELAY(160);
        R_H=DHT11_rec_byte();    //����ʪ�ȸ߰�λ  
        R_L=DHT11_rec_byte();    //����ʪ�ȵͰ�λ 
        T_H=DHT11_rec_byte();    //�����¶ȸ߰�λ  
        T_L=DHT11_rec_byte();    //�����¶ȵͰ�λ   
        revise=DHT11_rec_byte(); //����У��λ
       	GCC_DELAY(50);     //����
        if((R_H+R_L+T_H+T_L)==revise)      //У��
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