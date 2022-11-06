#include <STC12C5A60S2.H>
#include "uart.h"
#include "beep.h"
#include "1602.h"
#include "key.h"
#include "delay.h"
#include "servo.h"
#define uchar unsigned char
#define uint unsigned int 
#define FON P22==1
#define FOFF P22==0
volatile uchar Bio_Buffer[32];//防优化

extern login;
extern usercode;
extern lastkey;

//FINGERPRINT通信协议定义

code uchar Bio_Get_Device[10] ={0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00,0x00,0x1b};//口令验证
code uchar Bio_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  //协议包头
code uchar Bio_Get_Img[6] = {0x01,0x00,0x03,0x01,0x00,0x05};    //获得指纹图像
code uchar Bio_Get_Templete_Count[6] ={0x01,0x00,0x03,0x1D,0x00,0x21 }; //获得模版总数
code uchar Bio_Search[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x03,0xE7,0x00,0xF8}; //搜索指纹搜索范围0 - 999,使用BUFFER1中的特征码搜索
code uchar Bio_Search_0_9[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x00,0x13,0x00,0x21}; //搜索0-9号指纹
code uchar Bio_Img_To_Buffer1[7]={0x01,0x00,0x04,0x02,0x01,0x00,0x08}; //将图像放入到BUFFER1
code uchar Bio_Img_To_Buffer2[7]={0x01,0x00,0x04,0x02,0x02,0x00,0x09}; //将图像放入到BUFFER2
code uchar Bio_Reg_Model[6]={0x01,0x00,0x03,0x05,0x00,0x09}; //将BUFFER1跟BUFFER2合成特征模版
code uchar Bio_Delete_All_Model[6]={0x01,0x00,0x03,0x0d,0x00,0x11};//删除指纹模块里所有的模版
volatile uchar  Bio_Save_Finger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};//将BUFFER1中的特征码存放到指定的位置
//volatile:系统总是重新从它所在的内存读取数据，即使它前面的指令刚刚从该处读取过数据
/*------------------ FINGERPRINT命令字 --------------------------*/


 finger_detect(void) //77	后续返回usernum
{
	if(P22==1)
	{
		beep();
		
		return 77;
	}
	else
		return 0;

}
void BIO_Cmd_Send_Pack_Head(void)
{
	int i;	
	for(i=0;i<6;i++) //包头
   {
     Send_Byte(Bio_Pack_Head[i]);   
    }		
}

//发送指令
void BIO_Cmd_Check(void)
{
	int i=0;
	BIO_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<10;i++)
	 {		
		Send_Byte(Bio_Get_Device[i]);
	  }
}

//接收反馈数据缓冲
void BIO_Receive_Data(uchar ucLength)
{
	uchar n;

	for (n=0;n<ucLength;n++)
	{
		
		Bio_Buffer[n] = Uart_Receive_Byte();
		Send_Byte(Bio_Buffer[n]);
	}

}

//FINGERPRINT_获得指纹图像命令
void BIO_Cmd_Get_Img(void)
{
    uchar i;
    BIO_Cmd_Send_Pack_Head(); //发送通信协议包头
    for(i=0;i<6;i++) //发送命令 0x1d
	{
       Send_Byte(Bio_Get_Img[i]);
	}
}

//图像转换成特征码存放在Buffer1中
void BIO_Cmd_Img_To_Buffer1(void)
{
 	uchar i;
	BIO_Cmd_Send_Pack_Head(); //发送通信协议包头      
   	for(i=0;i<7;i++)   //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
     {
      Send_Byte(Bio_Img_To_Buffer1[i]);
   	  }
}

//将图像转换成特征码存放在Buffer2中
void BIO_Cmd_Img_To_Buffer2(void)
{
     uchar i;
     for(i=0;i<6;i++)    //发送包头
	 {
    	Send_Byte(Bio_Pack_Head[i]);   
   	 }
     for(i=0;i<7;i++)   //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
      {
      	Send_Byte(Bio_Img_To_Buffer2[i]);
   	  }
}


//搜索全部用户999枚
void BIO_Cmd_Search_Finger(void)
{
       uchar i;	   	    
			 BIO_Cmd_Send_Pack_Head(); //发送通信协议包头
       for(i=0;i<11;i++)
           {
    	      Send_Byte(Bio_Search[i]);   
   		   }
}

//将BUFFER1跟BUFFER2合成特征模版
void BIO_Cmd_Reg_Model(void)
{
       uchar i;	   
	    
			 BIO_Cmd_Send_Pack_Head(); //发送通信协议包头

       for(i=0;i<6;i++)
           {
    	      Send_Byte(Bio_Reg_Model[i]);   
   		   }
}

//删除指纹模块里的所有指纹模版
void BIO_Cmd_Delete_All_Model(void)
{
     uchar i;    
    for(i=0;i<6;i++) //包头
      Send_Byte(Bio_Pack_Head[i]);   
    for(i=0;i<6;i++) //命令合并指纹模版
	   {
      Send_Byte(Bio_Delete_All_Model[i]);   
		 }	
}


//保存指纹
void BIO_Cmd_Save_Finger( uint storeID )
{
       unsigned long temp = 0;
		   uchar i;
       Bio_Save_Finger[5] =(storeID&0xFF00)>>8;
       Bio_Save_Finger[6] = (storeID&0x00FF);
		   for(i=0;i<7;i++)   //计算校验和
		   	   temp = temp + Bio_Save_Finger[i]; 
		   Bio_Save_Finger[7]=(temp & 0x00FF00) >> 8; //存放校验数据
		   Bio_Save_Finger[8]= temp & 0x0000FF;		   
       BIO_Cmd_Send_Pack_Head(); //发送通信协议包头	
       for(i=0;i<9;i++)  
      		Send_Byte(Bio_Save_Finger[i]);      //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
}
//***********************************************************************************************************************************************************
//添加指纹

void BIO_Add_Fingerprint(int login)	  //Q:用户退出
{
	
	uchar *p=Bio_Buffer;
	ES=0;
	lastkey=-1;
	LCD_write_line(1,"     FINGER ADD     ");	//beep:continue beeps(2):retry  bee():done
	LCD_write_line(2,"     PRESS ENTER    ");
	
	while(waitkey(0)== 15)
	{	
		delay300(2);
		LCD_write_line(1,"  1- Please Finger  ");
		LCD_write_line(2,"                    ");
		LCD_write_line(3,"                    ");
		LCD_write_line(4,"                    ");
//		while(KEY_CANCEL == 1)
//		{
//		//按下返回键退出录入返回fingerID调整状态   
//			if(KEY_CANCEL == 0) 
//			{
//				while(KEY_CANCEL==0);
//				break;
//			}
			
			BIO_Cmd_Get_Img(); //获得指纹图像
			BIO_Receive_Data(12);
			delay100(1);
			Send_Byte(Bio_Buffer[9]);
			
	
		
				//判断接收到的确认码,等于0指纹获取成功
			if(Bio_Buffer[9]==0)
			{
				beeps(2);
				delay100(1);
				BIO_Cmd_Img_To_Buffer1();
				BIO_Receive_Data(12);
				LCD_write_line(2,"   Scan Successful  ");

				delay300(3);
				LCD_write_line(3,"   2-Please Finger  ");
				LCD_write_line(3,"                    ");
				LCD_write_line(3,"                   ");
				waitkey(0);
				while(FON)
				{
//					if(KEY_CANCEL == 0) 
//					{
//						while(KEY_CANCEL==0);
//						break;
//					}
					BIO_Cmd_Get_Img(); //获得指纹图像
					BIO_Receive_Data(12);
					
					//判断接收到的确认码,等于0指纹获取成功
					if(Bio_Buffer[9]==0)
					{
						
						delay300(1);
						beeps(2);
						BIO_Cmd_Img_To_Buffer2();
				  		BIO_Receive_Data(12);
						BIO_Cmd_Reg_Model();//转换成特征码
	         			BIO_Receive_Data(12); 
//					  	BIO_Cmd_Save_Finger(finger_id); 
						if(login==1)
							BIO_Cmd_Save_Finger(0);
						else
						{
							BIO_Cmd_Save_Finger(login*10);
						
						}
						
//						BIO_Cmd_Save_Finger(login);      						
	          			BIO_Receive_Data(12);
						
						LCD_write_line(3,"   Fully Scanned    ");
						LCD_write_line(4,"   Ur ID:           ");
						if(login>=1)
						{
							DisplayOneChar(11, 4, login+'0');
						}
							
						
						delay300(10);

				    	break;
				  	}
					waitkey(0);
					
				}
				 beeps(3); 

			}
			bee(2);
	}
	ES=1;
	delay300(2);	
}		

//搜索指纹
bit BIO_Find_Fingerprint()
{

	uint find_fingerid = 0;
	uchar id_show[]={0,0,0};
	do
	{
//		LCD_write_line(1,"   Finger Pls   ");
//		LCD_write_line(2,"    Decting.... ");
		
		BIO_Cmd_Get_Img(); //获得指纹图像
		BIO_Receive_Data(12);		

		//判断确认码
		if(Bio_Buffer[9]==0)
		{	
//			beep();
			BIO_Cmd_Img_To_Buffer1();
			BIO_Receive_Data(12);		
			BIO_Cmd_Search_Finger();
			BIO_Receive_Data(16);			
			if(Bio_Buffer[9] == 0 ) //搜索到  
			{

//				LCD_write_line(1,"   Finger Pls  ");
//				LCD_write_line(2,"   Decting...  ");
				beep();					
				//拼接指纹ID数
//				find_fingerid = Bio_Buffer[10]*256 + Bio_Buffer[11];

				set_servo_value(13);
				delay300(2);
				set_servo_value(0);
				delay300(3);		
				return 1;

			   }
				else //没有找到
				{
//					LCD_write_line(1,"    INVAILD    ");
//					LCD_write_line(2,"    FINGER     ");
				 	bee(1);
					while(FON);
					return 0;
				}
			}		
		}while(1); //确少返回函数
}
//************************************************************************************************************************************************
/*
//删除所有存贮的指纹库
void BIO_Cmd_Delete_All_Data()
{
		uchar i=0;
				LCD_write_line(0x80,"   empty all    ",0,16);
				LCD_write_line(0xc0,"   Yes or no ?  ",0,16); 
		do
		 {
			if(KEY_OK==0)
			{
				while(KEY_OK==0);
				LCD_write_line(0x80,"   emptying     ",0,16);
				LCD_write_line(0xc0,"                ",0,16); 
				delay1(300);
				LCD1602_WriteCMD(0xc0);
				for(i=0;i<16;i++)
				 {
					LCD1602_WriteDAT(42);
					delay1(100);
				 }
				FINGERPRINT_0Cmd_Delete_All_Model();
				BIO_Receive_Data(12); 
				LCD_write_line(0x80,"   All empty    ",0,16);
				LCD_write_line(0xc0,"                ",0,16);
				beeps(3);
				break;
			}
		 }while(KEY_CANCEL==1);
}

void Device_Check(void)
{
		uchar i=0;
		BIO_RECEICE_BUFFER[9]=1;				           //串口数组第九位可判断是否通信正常
									   
		for(i=0;i<8;i++)						           //进度条式更新，看起来美观
		{
			LCD1602_WriteDAT(42);	                       //42对应ASIC码的 *
			delay1(200);						           //控制进度条速度
		}									
		BIO_Cmd_Check();									//单片机向指纹模块发送校对命令
		LCD_write_line(0xc0,"Device Failure",0,16);      //液晶先显示对接失败，如果指纹模块插对的话会将其覆盖	
		
		BIO_Receive_Data(12);							//将串口接收到的数据转存
 		if(Bio_RECEICE_BUFFER[9] == 0)					//判断数据低第9位是否接收到0
		{
			LCD_write_line(0xc0,"Device Accessed",0,16);	//符合成功条件则显示对接成功
		}
}

*/

