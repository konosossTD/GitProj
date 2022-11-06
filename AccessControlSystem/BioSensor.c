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
volatile uchar Bio_Buffer[32];//���Ż�

extern login;
extern usercode;
extern lastkey;

//FINGERPRINTͨ��Э�鶨��

code uchar Bio_Get_Device[10] ={0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00,0x00,0x1b};//������֤
code uchar Bio_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  //Э���ͷ
code uchar Bio_Get_Img[6] = {0x01,0x00,0x03,0x01,0x00,0x05};    //���ָ��ͼ��
code uchar Bio_Get_Templete_Count[6] ={0x01,0x00,0x03,0x1D,0x00,0x21 }; //���ģ������
code uchar Bio_Search[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x03,0xE7,0x00,0xF8}; //����ָ��������Χ0 - 999,ʹ��BUFFER1�е�����������
code uchar Bio_Search_0_9[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x00,0x13,0x00,0x21}; //����0-9��ָ��
code uchar Bio_Img_To_Buffer1[7]={0x01,0x00,0x04,0x02,0x01,0x00,0x08}; //��ͼ����뵽BUFFER1
code uchar Bio_Img_To_Buffer2[7]={0x01,0x00,0x04,0x02,0x02,0x00,0x09}; //��ͼ����뵽BUFFER2
code uchar Bio_Reg_Model[6]={0x01,0x00,0x03,0x05,0x00,0x09}; //��BUFFER1��BUFFER2�ϳ�����ģ��
code uchar Bio_Delete_All_Model[6]={0x01,0x00,0x03,0x0d,0x00,0x11};//ɾ��ָ��ģ�������е�ģ��
volatile uchar  Bio_Save_Finger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};//��BUFFER1�е��������ŵ�ָ����λ��
//volatile:ϵͳ�������´������ڵ��ڴ��ȡ���ݣ���ʹ��ǰ���ָ��ոմӸô���ȡ������
/*------------------ FINGERPRINT������ --------------------------*/


 finger_detect(void) //77	��������usernum
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
	for(i=0;i<6;i++) //��ͷ
   {
     Send_Byte(Bio_Pack_Head[i]);   
    }		
}

//����ָ��
void BIO_Cmd_Check(void)
{
	int i=0;
	BIO_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
	for(i=0;i<10;i++)
	 {		
		Send_Byte(Bio_Get_Device[i]);
	  }
}

//���շ������ݻ���
void BIO_Receive_Data(uchar ucLength)
{
	uchar n;

	for (n=0;n<ucLength;n++)
	{
		
		Bio_Buffer[n] = Uart_Receive_Byte();
		Send_Byte(Bio_Buffer[n]);
	}

}

//FINGERPRINT_���ָ��ͼ������
void BIO_Cmd_Get_Img(void)
{
    uchar i;
    BIO_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
    for(i=0;i<6;i++) //�������� 0x1d
	{
       Send_Byte(Bio_Get_Img[i]);
	}
}

//ͼ��ת��������������Buffer1��
void BIO_Cmd_Img_To_Buffer1(void)
{
 	uchar i;
	BIO_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ      
   	for(i=0;i<7;i++)   //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
     {
      Send_Byte(Bio_Img_To_Buffer1[i]);
   	  }
}

//��ͼ��ת��������������Buffer2��
void BIO_Cmd_Img_To_Buffer2(void)
{
     uchar i;
     for(i=0;i<6;i++)    //���Ͱ�ͷ
	 {
    	Send_Byte(Bio_Pack_Head[i]);   
   	 }
     for(i=0;i<7;i++)   //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
      {
      	Send_Byte(Bio_Img_To_Buffer2[i]);
   	  }
}


//����ȫ���û�999ö
void BIO_Cmd_Search_Finger(void)
{
       uchar i;	   	    
			 BIO_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
       for(i=0;i<11;i++)
           {
    	      Send_Byte(Bio_Search[i]);   
   		   }
}

//��BUFFER1��BUFFER2�ϳ�����ģ��
void BIO_Cmd_Reg_Model(void)
{
       uchar i;	   
	    
			 BIO_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ

       for(i=0;i<6;i++)
           {
    	      Send_Byte(Bio_Reg_Model[i]);   
   		   }
}

//ɾ��ָ��ģ���������ָ��ģ��
void BIO_Cmd_Delete_All_Model(void)
{
     uchar i;    
    for(i=0;i<6;i++) //��ͷ
      Send_Byte(Bio_Pack_Head[i]);   
    for(i=0;i<6;i++) //����ϲ�ָ��ģ��
	   {
      Send_Byte(Bio_Delete_All_Model[i]);   
		 }	
}


//����ָ��
void BIO_Cmd_Save_Finger( uint storeID )
{
       unsigned long temp = 0;
		   uchar i;
       Bio_Save_Finger[5] =(storeID&0xFF00)>>8;
       Bio_Save_Finger[6] = (storeID&0x00FF);
		   for(i=0;i<7;i++)   //����У���
		   	   temp = temp + Bio_Save_Finger[i]; 
		   Bio_Save_Finger[7]=(temp & 0x00FF00) >> 8; //���У������
		   Bio_Save_Finger[8]= temp & 0x0000FF;		   
       BIO_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ	
       for(i=0;i<9;i++)  
      		Send_Byte(Bio_Save_Finger[i]);      //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
}
//***********************************************************************************************************************************************************
//���ָ��

void BIO_Add_Fingerprint(int login)	  //Q:�û��˳�
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
//		//���·��ؼ��˳�¼�뷵��fingerID����״̬   
//			if(KEY_CANCEL == 0) 
//			{
//				while(KEY_CANCEL==0);
//				break;
//			}
			
			BIO_Cmd_Get_Img(); //���ָ��ͼ��
			BIO_Receive_Data(12);
			delay100(1);
			Send_Byte(Bio_Buffer[9]);
			
	
		
				//�жϽ��յ���ȷ����,����0ָ�ƻ�ȡ�ɹ�
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
					BIO_Cmd_Get_Img(); //���ָ��ͼ��
					BIO_Receive_Data(12);
					
					//�жϽ��յ���ȷ����,����0ָ�ƻ�ȡ�ɹ�
					if(Bio_Buffer[9]==0)
					{
						
						delay300(1);
						beeps(2);
						BIO_Cmd_Img_To_Buffer2();
				  		BIO_Receive_Data(12);
						BIO_Cmd_Reg_Model();//ת����������
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

//����ָ��
bit BIO_Find_Fingerprint()
{

	uint find_fingerid = 0;
	uchar id_show[]={0,0,0};
	do
	{
//		LCD_write_line(1,"   Finger Pls   ");
//		LCD_write_line(2,"    Decting.... ");
		
		BIO_Cmd_Get_Img(); //���ָ��ͼ��
		BIO_Receive_Data(12);		

		//�ж�ȷ����
		if(Bio_Buffer[9]==0)
		{	
//			beep();
			BIO_Cmd_Img_To_Buffer1();
			BIO_Receive_Data(12);		
			BIO_Cmd_Search_Finger();
			BIO_Receive_Data(16);			
			if(Bio_Buffer[9] == 0 ) //������  
			{

//				LCD_write_line(1,"   Finger Pls  ");
//				LCD_write_line(2,"   Decting...  ");
				beep();					
				//ƴ��ָ��ID��
//				find_fingerid = Bio_Buffer[10]*256 + Bio_Buffer[11];

				set_servo_value(13);
				delay300(2);
				set_servo_value(0);
				delay300(3);		
				return 1;

			   }
				else //û���ҵ�
				{
//					LCD_write_line(1,"    INVAILD    ");
//					LCD_write_line(2,"    FINGER     ");
				 	bee(1);
					while(FON);
					return 0;
				}
			}		
		}while(1); //ȷ�ٷ��غ���
}
//************************************************************************************************************************************************
/*
//ɾ�����д�����ָ�ƿ�
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
		BIO_RECEICE_BUFFER[9]=1;				           //��������ھ�λ���ж��Ƿ�ͨ������
									   
		for(i=0;i<8;i++)						           //������ʽ���£�����������
		{
			LCD1602_WriteDAT(42);	                       //42��ӦASIC��� *
			delay1(200);						           //���ƽ������ٶ�
		}									
		BIO_Cmd_Check();									//��Ƭ����ָ��ģ�鷢��У������
		LCD_write_line(0xc0,"Device Failure",0,16);      //Һ������ʾ�Խ�ʧ�ܣ����ָ��ģ���ԵĻ��Ὣ�串��	
		
		BIO_Receive_Data(12);							//�����ڽ��յ�������ת��
 		if(Bio_RECEICE_BUFFER[9] == 0)					//�ж����ݵ͵�9λ�Ƿ���յ�0
		{
			LCD_write_line(0xc0,"Device Accessed",0,16);	//���ϳɹ���������ʾ�Խӳɹ�
		}
}

*/

