#include "usartx.h"
unsigned char DM_Action0[5]   = { 0xff, 0x09, 0x00, 0x00, 0x00 };               //������0
unsigned char DM_Action1[5]   = { 0xff, 0x09, 0x00, 0x01, 0x00 };               //������1
unsigned char DM_Action2[5]   = { 0xff, 0x09, 0x00, 0x02, 0x00 };               //������2
unsigned char DM_Action3[5]   = { 0xff, 0x09, 0x00, 0x03, 0x00 };               //������3
unsigned char DM_Action4[5]   = { 0xff, 0x09, 0x00, 0x04, 0x00 };               //������4
unsigned char DM_Action5[5]   = { 0xff, 0x09, 0x00, 0x05, 0x00 };               //������5
//int Usart_Receive3;
int speed_x,speed_y,speed_z;
u8 rxbuf3[10]   ={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart4����һ���ֽ�
*********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	
}
/**************************************************************************
�������ܣ�����3��ʼ��
��ڲ�����pclk2:PCLK2 ʱ��Ƶ��(Mhz)    bound:������
����  ֵ����
**************************************************************************/
void uart3_init(u32 pclk2,u32 bound)
{  	 
float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
  mantissa<<=4;
	mantissa+=fraction; 
	

	RCC->APB2ENR|=1<<0;    //��������ʱ��
	RCC->APB2ENR|=1<<4;   //ʹ��PORTC��ʱ��  
	RCC->APB1ENR|=1<<18;  //ʹ�ܴ���ʱ�� 
	GPIOC->CRH&=0XFFFF00FF; 
	GPIOC->CRH|=0X00008B00;//IO״̬����
	GPIOC->ODR|=1<<10;	 
  AFIO->MAPR|=1<<4;      //������ӳ��

	RCC->APB1RSTR|=1<<18;   //��λ����1
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	   	   
	//����������
 	USART3->BRR=mantissa; // ����������	 
	USART3->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж�
	USART3->CR1|=1<<8;    //PE�ж�ʹ��
	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,USART3_IRQn,2);//��2��������ȼ� 
}

/**************************************************************************
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
//int USART3_IRQHandler(void)
//{	
//	if(USART3->SR&(1<<5))//���յ�����
//	{	      
//		static u8 i,j,temp,Receive[50];
//		static float Data;
//		temp=USART3->DR;
//		rxbuf3[i]=temp;
//		i++;

//		if(rxbuf3[1]==0xfe&&rxbuf3[2]==0x01&&rxbuf3[9]==0x00)  
//    {
//			speed_x=rxbuf3[3];
//			speed_y=rxbuf3[5];
//			speed_z=rxbuf3[7];
//		}
//   }
//return 0;	
//}
int USART3_IRQHandler(void)
{	
	if(USART3->SR&(1<<5))//���յ�����
	{	      
				u8 temp;
				static u8 count;//last_data,last_last_data;//Usart_ON_Count;
//		    if(Usart_ON_Flag==0)
//				{	
//		    if(++Usart_ON_Count>10)Usart_ON_Flag=1;  //����10�ν��봮�ڽ����жϣ�ʹ�ܴ��ڿ���
//				}
				temp=USART3->DR;  //��ȡ���ռĴ�����ֵ
				 if(Usart_Flag==0)
					{	
					if(temp==0xfe||temp==0xff)  //����ͷ
					Usart_Flag=1,count=0;	
					}
				 if(Usart_Flag==1)
					{	
						rxbuf3[count]=temp;     //���βɼ�����
						count++;						
//						if(count==10)Usart_Flag=0;
					
						if(count>=10&&rxbuf3[1]==0xfe&&rxbuf3[2]==0x01&&rxbuf3[9]==0x00)					
						{
//								UART2_DM_ReportData(rxbuf3);
								speed_x=rxbuf3[3];
								speed_y=rxbuf3[5];
								speed_z=rxbuf3[7];
								printf("%d:%d:%d:\n",speed_x,speed_y,speed_z);
								Usart_Flag=0;
								count=0;
								rxbuf3[0]=0;
						}
					}
//					last_last_data=last_data;  //�������ϴε�����
//					last_data=temp;            //�����ϴε�����
   }
	 else 
	 {speed_x=0,speed_y=0,speed_z=0;}
		 
return 0;	
}
void UART2_DM_ReportData(unsigned char data[])
{
	 char i;
		for( i = 0 ; i <= 9; i++) 
		{
		while((USART2->SR&0X40)==0);  
		USART2->DR = data[i]; 
		}
}
/**************************************************************************
�������ܣ�����3���Ͷ�����������ָ���
��ڲ�������
����  ֵ����
**************************************************************************/
void UART_DM_ReportData(unsigned char data[])
{
	 char i;
		for( i = 0 ; i < 5; i++) 
		{
		while((USART3->SR&0X40)==0);  
		USART3->DR = data[i]; 
		}
}



