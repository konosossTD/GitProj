#include "usartx.h"
unsigned char DM_Action0[5]   = { 0xff, 0x09, 0x00, 0x00, 0x00 };               //动作组0
unsigned char DM_Action1[5]   = { 0xff, 0x09, 0x00, 0x01, 0x00 };               //动作组1
unsigned char DM_Action2[5]   = { 0xff, 0x09, 0x00, 0x02, 0x00 };               //动作组2
unsigned char DM_Action3[5]   = { 0xff, 0x09, 0x00, 0x03, 0x00 };               //动作组3
unsigned char DM_Action4[5]   = { 0xff, 0x09, 0x00, 0x04, 0x00 };               //动作组4
unsigned char DM_Action5[5]   = { 0xff, 0x09, 0x00, 0x05, 0x00 };               //动作组5
//int Usart_Receive3;
int speed_x,speed_y,speed_z;
u8 rxbuf3[10]   ={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/**************************实现函数**********************************************
*功    能:		usart4发送一个字节
*********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	
}
/**************************************************************************
函数功能：串口3初始化
入口参数：pclk2:PCLK2 时钟频率(Mhz)    bound:波特率
返回  值：无
**************************************************************************/
void uart3_init(u32 pclk2,u32 bound)
{  	 
float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction; 
	

	RCC->APB2ENR|=1<<0;    //开启辅助时钟
	RCC->APB2ENR|=1<<4;   //使能PORTC口时钟  
	RCC->APB1ENR|=1<<18;  //使能串口时钟 
	GPIOC->CRH&=0XFFFF00FF; 
	GPIOC->CRH|=0X00008B00;//IO状态设置
	GPIOC->ODR|=1<<10;	 
  AFIO->MAPR|=1<<4;      //部分重映射

	RCC->APB1RSTR|=1<<18;   //复位串口1
	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
	//波特率设置
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断
	USART3->CR1|=1<<8;    //PE中断使能
	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,USART3_IRQn,2);//组2，最低优先级 
}

/**************************************************************************
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/
//int USART3_IRQHandler(void)
//{	
//	if(USART3->SR&(1<<5))//接收到数据
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
	if(USART3->SR&(1<<5))//接收到数据
	{	      
				u8 temp;
				static u8 count;//last_data,last_last_data;//Usart_ON_Count;
//		    if(Usart_ON_Flag==0)
//				{	
//		    if(++Usart_ON_Count>10)Usart_ON_Flag=1;  //超过10次进入串口接收中断，使能串口控制
//				}
				temp=USART3->DR;  //读取接收寄存器的值
				 if(Usart_Flag==0)
					{	
					if(temp==0xfe||temp==0xff)  //数据头
					Usart_Flag=1,count=0;	
					}
				 if(Usart_Flag==1)
					{	
						rxbuf3[count]=temp;     //依次采集数据
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
//					last_last_data=last_data;  //保存上上次的数据
//					last_data=temp;            //保存上次的数据
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
函数功能：串口3发送舵机动作组控制指令函数
入口参数：无
返回  值：无
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



