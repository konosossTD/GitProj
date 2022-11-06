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
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      

	while((USART2->SR&0X40)==0);
	USART2->DR = (u8) ch;      
  return ch;
}
#endif 

int Usart_Receive;
/**************************实现函数**********************************************
*功    能:		usart1发送一个字节
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
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB2ENR|=1<<14;  //使能串口时钟 
	GPIOA->CRH&=0XFFFFF00F;//IO状态设置
	GPIOA->CRH|=0X000008B0;//IO状态设置 
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X200C;  //1位停止,无校验位.
}



//////////////////////////////////////////////////////////////////
/**************************实现函数**********************************************
*功    能:		usart2发送一个字节
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
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB1ENR|=1<<17;  //使能串口时钟 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO状态设置
	GPIOA->ODR|=1<<10;	  
	RCC->APB1RSTR|=1<<18;   //复位串口1
	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
	//波特率设置
 	USART2->BRR=mantissa; // 波特率设置	 
	USART2->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断
	USART2->CR1|=1<<8;    //PE中断使能
	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,USART2_IRQn,2);//组2，最低优先级 
}
/**************************************************************************
函数功能：串口2接收中断
入口参数：无
返回  值：无
**************************************************************************/
int USART2_IRQHandler(void)
{	
	
	if(USART2->SR&(1<<5))//接收到数据
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

		
		if(Run_Flag==0)//速度控制模式
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
    else if(Run_Flag==1)//速度控制模式
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
		
		//以下是与APP调试界面通讯
		if(Usart_Receive==0x7B) Flag_PID=1;   //APP参数指令起始位
		if(Usart_Receive==0x7D) Flag_PID=2;   //APP参数指令停止位

		 if(Flag_PID==1)  //采集数据
		 {
			Receive[i]=Usart_Receive;
			i++;
		 }
		 if(Flag_PID==2)  //分析数据
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
							 case 0x30:  RC_Velocity=Data;break;//速度设定值
							 case 0x31:  RC_Position=Data;break;//位置步长设定值
							 case 0x32:  Position_KP=Data;break;//位置PID系数
							 case 0x33:  Position_KI=Data;break;//
							 case 0x34:  Position_KD=Data;break;//
							 case 0x37:  break; //预留
							 case 0x38:  break; //预留
						 }
					 }				 
					 Flag_PID=0;//相关标志位清零
					 i=0;
					 j=0;
					 Data=0;
					 memset(Receive, 0, sizeof(u8)*50);//数组清零
		 } 	 
   }
	//Flag_Direction=click_RC();
return 0;	
}



/**************************************************************************
函数功能：串口扫描只执行一次串口的操作
**************************************************************************/
u8 click_RC (void)
{
			static u8 flag_key=1;//按键按松开标志
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
				  
					return temp;	// 按键按下
			}
			else if(Usart_Receive==0x5A)			flag_key=1;
			return 0;//无按键按下
}

