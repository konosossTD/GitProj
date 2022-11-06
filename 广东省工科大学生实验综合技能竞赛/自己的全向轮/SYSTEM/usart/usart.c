#include "usart.h"
#include "control.h"

extern int f;
int f1=0;
int f2=0;
int f3=0;
int f4=0;
int f5=0;
int erwei_flag=0;
extern u8 Position_Move_Line_stop_flag;
u8 Flag_Direction1=0;
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      

	while((USART2->SR&0X40)==0);
	USART2->DR = (u8) ch;      
  return ch;
}
#endif 

int Usart_Receive;
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart1����һ���ֽ�
*********************************************************************************/
void usart1_send(u8 data)
{
	USART1->DR = data;
	while((USART1->SR&0x40)==0);	
}
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
  mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB2ENR|=1<<14;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRH&=0XFFFFF00F;//IO״̬����
	GPIOA->CRH|=0X000008B0;//IO״̬���� 
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X200C;  //1λֹͣ,��У��λ.
}



//////////////////////////////////////////////////////////////////
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart2����һ���ֽ�
*********************************************************************************/
void usart2_send(u8 data)
{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	
}
void uart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
  mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB1ENR|=1<<17;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO״̬����
	GPIOA->ODR|=1<<10;	  
	RCC->APB1RSTR|=1<<18;   //��λ����1
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=mantissa; // ����������	 
	USART2->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж�
	USART2->CR1|=1<<8;    //PE�ж�ʹ��
	USART2->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,USART2_IRQn,2);//��2��������ȼ� 
}
/**************************************************************************
�������ܣ�����2�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
int USART2_IRQHandler(void)
{	
	
	if(USART2->SR&(1<<5))//���յ�����
	{	      
		static u8 Flag_PID,i,j,Receive[50];
		static float Data;
		Usart_Receive=USART2->DR;
		if(Usart_Receive==0x4D)
		{Position_Move_Line_stop_flag=0;f1=1;}
		if(Usart_Receive==0x12&&f2==0)
		{erwei_flag=12;f2=1;}
		if(Usart_Receive==0x22&&f3==0)
		{erwei_flag=22;f3=1;}
		if(Usart_Receive==0x21&&f4==0)
		{erwei_flag=21;f4=1;}
		if(Usart_Receive==0x11&&f5==0)
		{erwei_flag=11;f5=1;}
		
	
		 if(Usart_Receive==0x58) 	PCout(3)=!PCout(3);

		
		if(Run_Flag==0)//�ٶȿ���ģʽ
		{			
						if(Usart_Receive>=0x41&&Usart_Receive<=0x4C)  
						{	
							Flag_Direction=Usart_Receive-0x40;
						}
						else	if(Usart_Receive<=12)   
						{			
							Flag_Direction=Usart_Receive;
						}	
						else  Flag_Direction=0;
	  }	
    else if(Run_Flag==1)//�ٶȿ���ģʽ
		{			
						if(Usart_Receive>=0x41&&Usart_Receive<=0x4C)  
						{	
							Flag_Direction=Usart_Receive-0x40;
						}
						else	if(Usart_Receive<=12)   
						{			
							Flag_Direction=Usart_Receive;
						}	
						else  Flag_Direction=0;
	  }	
		
		//��������APP���Խ���ͨѶ
		if(Usart_Receive==0x7B) Flag_PID=1;   //APP����ָ����ʼλ
		if(Usart_Receive==0x7D) Flag_PID=2;   //APP����ָ��ֹͣλ

		 if(Flag_PID==1)  //�ɼ�����
		 {
			Receive[i]=Usart_Receive;
			i++;
		 }
		 if(Flag_PID==2)  //��������
		 {
			     if(Receive[3]==0x50) 	 PID_Send=1;
					 else  if(Receive[3]==0x57) 	 Flash_Send=1;
					 else  if(Receive[1]!=0x23) 
					 {								
						for(j=i;j>=4;j--)
						{
						  Data+=(Receive[j-1]-48)*pow(10,i-j);
						}
						switch(Receive[1])
						 {
							 case 0x30:  RC_Velocity=Data;break;//�ٶ��趨ֵ
							 case 0x31:  RC_Position=Data;break;//λ�ò����趨ֵ
							 case 0x32:  Position_KP=Data;break;//λ��PIDϵ��
							 case 0x33:  Position_KI=Data;break;//
							 case 0x34:  Position_KD=Data;break;//
							 case 0x37:  break; //Ԥ��
							 case 0x38:  break; //Ԥ��
						 }
					 }				 
					 Flag_PID=0;//��ر�־λ����
					 i=0;
					 j=0;
					 Data=0;
					 memset(Receive, 0, sizeof(u8)*50);//��������
		 } 	 
   }
	//Flag_Direction=click_RC();
return 0;	
}



/**************************************************************************
�������ܣ�����ɨ��ִֻ��һ�δ��ڵĲ���
**************************************************************************/
u8 click_RC (void)
{
			static u8 flag_key=1;//�������ɿ���־
	    u8 temp;
			if(flag_key&&Usart_Receive!=0x5A)
			{
					flag_key=0;
				  if(Usart_Receive>=0x01&&Usart_Receive<=0x12)temp=Usart_Receive;
				  else	if(Usart_Receive>=0x41&&Usart_Receive<=0x4E)temp=Usart_Receive-0x40;	
					else 	temp=0;
				  //if (temp==13)
						//Position_Move_Line_stop_flag=;
					//temp=12;
				  
					return temp;	// ��������
			}
			else if(Usart_Receive==0x5A)			flag_key=1;
			return 0;//�ް�������
}

