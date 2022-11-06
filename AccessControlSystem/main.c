#include "1602.h"
#include "delay.h"
#include "beep.h"
#include "uart.h"
#include "key.h"
#include "func.h"
#include "user.h"
#include "ds1302.h"
#include "BioSensor.h"
#include <STC12C5A60S2.H>
#include "rc522.h"
#include "eeprom.h"
#include "servo.h"
#include "timer0.h"

#define uchar unsigned char
#define uint unsigned int 
	
//uchar Card_type[20];

extern uint root[6]={5,5,4,8,9,4};	//su
														//us      column/	pwd[0-5]	[id]	[1/0(active/deactive)][����ָ��ID]
extern uint usercode[3][8]={
							{1,1,1,1,1,1,2,0}, 
							{2,2,2,2,2,2,3,0}, 
							{3,3,3,3,3,3,4,0}};
extern uint box[6]={0,0,0,0,0,0};					//codebuffer												
uchar flag='x';										//��ɾ��־λ
extern int lastkey=-1;//
bit homepage;
extern int login=0; //current user	ֻ�ɱ�pwdcheck��ֵ
int usr;
uint i;
//def 								11&12:backspace  
//									13&14:quit  
//									15&16��enter
uchar byte=0x77;
void main(){
//������������������������LCD,clock,uart,iic,fingerprint,keypadinition������������������������//	

	beep();
	eeprom_init();
	Clock_Init();
	Timer0Init();
	Init_Lcd();
	Uart_Init();
	set_servo_value(0);
//	Clock_write_time(); 
	mainpage();

	beeps(2);

//������������������������������������mainpage������������������������������������������������//
	while(1)
	{	

		
		homepage=1;
		lastkey=-1;
		if(waitkey(1)==77)	
		{
			lastkey=-1;
			continue;
		}
//		else if(lastkey==0)
//		{
//			PCON|=0x02;
//			lastkey=-1;

//		}
		
		
		
//		rfidunlockɨ��
//		ָ��unlockɨ��
//		����ɨ��

//		waitforaccess(buffer);
//		Send_Byte('s');
//		Send_Str("\n");
//		Send_Byte(root[i]);
		switch(trans(lastkey))
		{
			
			case 1://����ģʽ

				password(); //lcdupdate
				
				login=pwdcheck(1);
				while(login==0)
				{	bee(1);
					LCD_write_line(1,"--------WRONG-------");
					LCD_write_line(2,"PassWord:******     ");
					login=pwdcheck(1);
				}
				if(login>0) //�ж��û�(����)
				{	
					
					beep();
					LCD_write_line(1,"    Welcome NO. !   ");
					DisplayOneChar(15, 0,login+'0');//��ȷ
					set_servo_value(13);
					delay300(2);
					set_servo_value(0);
					beeps(6);

					
				}
				else
				{
					bee(1);//����					
				}
				
				mainpage();
				break;
			case 2://15&16 ����
				password();
				login=pwdcheck(1);
				if(login<1){bee(1);mainpage();break;}
				menu();//��֤����  ����ָ�ƶ�����
				lastkey=-1;
				while(1)
				{
					lastkey=waitkey(0);
					delay50(1);
					if(lastkey==13 ||lastkey==14){break;}
					if(lastkey==1 ||lastkey==2||lastkey==3){break;}
				}
				
				
				flag='x'; 
				if(lastkey==13 ||lastkey==14){mainpage();break;}
				switch(lastkey)
				{
					case 1:
//						settings();
						flag='P';//assword
						break;
					case 2:
//						settings();
						flag='F';//ingerprint
						break;
					case 3:
//						settings();
						flag='C';//ard
						break;
				}
				settings();
				delay50(1);
				while(1)
				{
					lastkey=waitkey(0);
					delay1(20);
					if(lastkey==13 ||lastkey==14){break;}
					if(lastkey==1 ||lastkey==2){break;}
				}
				if(lastkey==13 ||lastkey==14){flag='x';mainpage();break;}
				switch(flag)
				{
					case 'P':
						if(lastkey==1)					//-����-CHANGE
						{
							chgpwd();
							lastkey=-1;
						}
						else if(lastkey==2 &&login==1)					//-����-Delete
						{
							
							LCD_write_line(1,"  SELECT  USER  ID ");
							LCD_write_line(2,"   [2]  [3]  [4]   ");
							usr=waitkey(0);
							if(usr==13||usr==14){break;}
							while(usr>8||usr<2)
							{
								bee(1);
								usr=waitkey(0);
							}
							if(usr>5||usr<=8)
							{
								usercode[usr-6][7]=1;
							}
//							Send_Byte(usr);
							usercode[usr-2][7]=0;
//							Send_Byte(usercode[usr][7]);
							LCD_write_line(1,"------SUCCEED------");
							LCD_write_line(2," NO.  ---- DELETED!");
							DisplayOneChar(5,1,usr+'0');
							LCD_write_line(3,"                   ");
							LCD_write_line(4,"                   ");
							bee(1);
						}
						mainpage();
						break;
							
					case 'F':
						if(lastkey==1&&login==1)//add
						{
//						Send_Byte(login);
						BIO_Add_Fingerprint(login);
						
						
						}
						else if(lastkey==2&&login==1)		//flag='x'
						{
							
							BIO_Cmd_Delete_All_Model();
							clear();
							LCD_write_line(4,"  ALL DELETED!!!   ");
							beeps(2);
						
						}
						else
						{
							if(login!=1){
							LCD_write_line(4,"   INVAILD USER!!! ");
							bee(3);
							mainpage();
							}
						
						
						break;
						}
					case 'C':
						if(lastkey==1)		//fid
						{	beeps(3);
							download();
							mainpage();
						}
						else			//flag='x'
						{
							
							upload();
							beeps(5);
						
						
						}
						
						break;
				}
				lastkey=-1;
//				date_update(0);
//				mainpage();
				break;
			case 9:

				mainpage();
				date_update(0);
				bee(1);
				break;
	
		}

	}
}

