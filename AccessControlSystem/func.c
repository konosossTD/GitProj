#include "delay.h"
#include "key.h"
#include "beep.h"
#include "1602.h"
#include "uart.h"
#include "user.h"
#include "func.h"
extern short lastkey;
#define uchar unsigned char
#define uint unsigned int 
extern login;
extern root[6];
extern usercode[3][8];
extern box[6];


//def 							11&12:backspace  
//									13&14:quit  
//									15&16：enter
short trans(int lastkey)
{
	if (lastkey<10 && lastkey>0)
		return 1;//密码比对模式
	else if (lastkey>14 &&lastkey<16)
		return 2;//设置
	else if  (lastkey>12 &&lastkey<15)
		return 9;//quit
	else
		return -1;
}


int pwdcheck(bit r)	// -1:error  0:wrong;  1:su;  2:user etc...  seg:   r:only root
{	
	int i;
	int j=0;
	int flag=-1;

	for(i=0;i<6;i++)
	{
		box[i]=waitkey(0); 
		delay100(1);

		while(box[i]>9 || box[i]<0)	//非正常输入
		{	
			
			if(box[i]==11||box[i]==12){

				if(i<=0)
				{	
					i=0;
					box[i]=waitkey(0);
					DisplayOneChar(8+i,1,':');
				}
				else{
					i--;
					
					DisplayOneChar(9+i,1,'*');
				}
			}
			if(box[i]==13||box[i]==14){return -1;}
			
			box[i]=waitkey(0);
		}
		DisplayOneChar(9+i,1,box[i]+0x30);//8&16reg

		if(box[i]!=root[i])
		{
			flag=0;
		}
		
	}

	if (flag==-1)
	{
		flag=1;
		return flag;
	}
	if(!r){return 0;}
	else if(flag==0)//开始用户匹配
	{	
		int u;
		Send_Byte(usercode[0][7]);Send_Byte(usercode[1][7]);Send_Byte(usercode[2][7]);
		
		for(j=0;j<3;j++)
		{
			if(usercode[j][7]==1)
			{
				
				flag=-1;
				u=usercode[j][6];
				Send_Byte(usercode[j][6]);
				for(i=0;i<6;i++)
				{
					
					Send_Byte(box[i]);
					Send_Byte(usercode[j][i]);
					
					if(box[i]!=usercode[j][i]){flag=0;}
				}
				if(flag==-1){flag=u;return flag;}  //返回值有问题 和外部没有建立联系 记得调试 //done
			}
			
		}
		
	}
	lastkey=-1;
	return 0;//
}


void chgpwd()
{	

	int i;
	int tmp;
	int usr=0;
	lastkey=-1;
//	Send_Byte(login);
	if(login==1)
	{
		LCD_write_line(1,"    CHOOSE  USERS   ");
		LCD_write_line(2,"     1.[ROOT]       ");
		LCD_write_line(3,"    [2] [3] [4]     ");
		LCD_write_line(4,"                    ");
	
		while(1){
			waitkey(0);
			if(lastkey==13 ||lastkey==14){mainpage();break;}
			if(lastkey>0 &&lastkey<5){usr=lastkey;break;}
		}
	}
	else
	{
		usr=login;
	}
	LCD_write_line(1,"   INPUT NEW CODE   ");
	LCD_write_line(2,"Password: ******    ");
	LCD_write_line(3,"                    ");
	LCD_write_line(4,"                    ");
	for(i=0;i<6;i++)
	{	
		Send_Byte(usr);
		Send_Byte(login);
		tmp=waitkey(0);
		delay100(1);
		while(tmp>=11)
		{
			bee(1);
			delay100(1);
			tmp=waitkey(0);
		}
		box[i]=tmp;
		if(usr==1)
		{
			root[i]=box[i];
		}
		else{
		
			usercode[usr-2][i]=box[i];
			usercode[usr-2][7]=1;
		}
		
		DisplayOneChar(10+i,1,box[i]+0x30);//8&16reg

	}

	
	
}

