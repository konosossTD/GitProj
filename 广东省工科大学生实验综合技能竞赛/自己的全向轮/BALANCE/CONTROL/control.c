#include "control.h"	
#include "filter.h"	
#include "delay.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/

u8 Flag_Target,Flag_Change;                             //��ر�־λ
u8 temp1;                                               //��ʱ����
u8 Position_Move_Line_count=0,Position_Move_Line_flag=0,Position_Move_Line_stop_flag=1;
u8 Openmv_Control_flag=0;
extern u8 Flag_Direction1;
float Voltage_Count,Voltage_All;  //��ѹ������ر���
float Gyro_K=0.004;       //�����Ǳ���ϵ��
int j;
int f=1;
int Flag_Jiasu;

extern int erwei_flag;

#define X_PARAMETER          (0.5f)  //1/2�ĸ�����             
#define Y_PARAMETER           (sqrt(3)/2.f)//����֮����3�ĸ�����      
#define L_PARAMETER            (1.0f)  

/**************************************************************************
�������ܣ�С���˶���ѧģ��
��ڲ�����X Y Z �����ٶȻ���λ��
����  ֵ����
**************************************************************************/
void Kinematic_Analysis(float Vx,float Vy,float Vz)//Vz=LW
{
        Target_A   = Vx + L_PARAMETER*Vz+gyro[2]*Gyro_K;//����������z�����ݣ�ʹ���ߵĸ�ֱ
        Target_B   = -X_PARAMETER*Vx + Y_PARAMETER*Vy + L_PARAMETER*Vz+gyro[2]*Gyro_K;//�ɼӿɲ��������������Ϊ��������ȽϾ�ȷ�������ϸ���
	      Target_C   = -X_PARAMETER*Vx - Y_PARAMETER*Vy + L_PARAMETER*Vz+gyro[2]*Gyro_K;
}
//void Kinematic_Analysis(float Vx,float Vy,float Vz)//Vz=LW
//{
//        Target_A   = Vx + L_PARAMETER*Vz;//����������z�����ݣ�ʹ���ߵĸ�ֱ
//        Target_B   = -X_PARAMETER*Vx + Y_PARAMETER*Vy + L_PARAMETER*Vz;
//	      Target_C   = -X_PARAMETER*Vx - Y_PARAMETER*Vy + L_PARAMETER*Vz;
//}
/**************************************************************************
�������ܣ���ȡλ�ÿ��ƹ����ٶ�ֵ
��ڲ�����X Y Z ����λ�ñ仯��
����  ֵ����
**************************************************************************/
void Kinematic_Analysis2(float Vx,float Vy,float Vz)
{
	      Rate_A   = Vx + L_PARAMETER*Vz;
        Rate_B   =-X_PARAMETER*Vx + Y_PARAMETER*Vy + L_PARAMETER*Vz;
	      Rate_C   =-X_PARAMETER*Vx - Y_PARAMETER*Vy + L_PARAMETER*Vz;
}
/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
         5ms��ʱ�ж���MPU6050��INT���Ŵ���
         �ϸ�֤���������ݴ����ʱ��ͬ��				 
**************************************************************************/
int EXTI15_10_IRQHandler(void) 
{    
	 if(INT==0)		
	{     
		  EXTI->PR=1<<15;                                                      //���LINE5�ϵ��жϱ�־λ  		
		  Flag_Target=!Flag_Target;
		if(delay_flag==1)
			 {
				 if(++delay_50==10)	 delay_50=0,delay_flag=0;                     //���������ṩ50ms�ľ�׼��ʱ
			 }                           					 //===10ms����һ��
			Read_DMP();
		if(Flag_Target==1)
     {
			Read_DMP();                  				 //===������̬	
			Encoder_A=Motor_A;            	 		//===ͨ��Ƶ�ʽ����ٶ�
	  	Encoder_B=Motor_B;           		 		//===ͨ��Ƶ�ʽ����ٶ�
	  	Encoder_C=Motor_C;            	 	 //===ͨ��Ƶ�ʽ����ٶ�
			Position_A+=4.8*Encoder_A*0.01;     //===ͨ��Ƶ�ʻ��ּ���λ��
	  	Position_B+=4.8*Encoder_B*0.01;    //===ͨ��Ƶ�ʻ��ּ���λ��
		  Position_C+=4.8*Encoder_C*0.01;    //===ͨ��Ƶ�ʻ��ּ���λ��
  		Led_Flash(100);                     //===LED��˸;����ģʽ 1s�ı�һ��ָʾ�Ƶ�״̬	
			Voltage_All+=Get_battery_volt();     //��β����ۻ�
			if(++Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//��ƽ��ֵ ��ȡ��ص�ѹ	       
		 if(Turn_Off(Voltage)==0)               //===�����ص�ѹ�������쳣
		 { 			 
				if(Run_Flag==0)//�ٶ�ģʽ
				{		
//							Flag_Direction=click_RC();
//					    if(Flag_Direction==13){Openmv_Control_flag=1;}
//							else{Position_Move_Line_flag=0;}
							Get_RC(Run_Flag);  //===���ں�CAN���ƶ�δʹ�ܣ����������ң��ָ��
							Motor_A=Target_A;//ֱ�ӵ���PWMƵ��
							Motor_B=Target_B;//ֱ�ӵ���PWMƵ��
							Motor_C=Target_C;//ֱ�ӵ���PWMƵ��
				}
				else//λ��ģʽ
				{    
							Flag_Direction=click_RC();
					    if(Flag_Direction==11){Position_Move_Line_flag=1;}
							//else{Position_Move_Line_flag=0;}
							Get_RC(Run_Flag);
					    
							Motor_A=-Position_PID_A(Position_A,Target_A)>>4;//A���λ�ÿ���
							Motor_B=-Position_PID_B(Position_B,Target_B)>>4;//B���λ�ÿ���
							Motor_C=-Position_PID_C(Position_C,Target_C)>>4;//C���λ�ÿ���

							if(myabs(Motor_A)<100&&myabs(Motor_B)<100&&myabs(Motor_C)<100)Max_Velocity=0;  //������ֹͣǰ���٣�ABS����ֵ����
							else
							{
							Max_Velocity+=12;  
							if(Max_Velocity>RC_Velocity)Max_Velocity=RC_Velocity;
							}
							if(rxbuf[0]!=2)  Count_Velocity();   //���ǵ���λ�ÿ��ƹ��̵��ٶȴ�С
							else 
							Xianfu_Velocity(Max_Velocity,Max_Velocity,Max_Velocity); //�ٶ��޷�
				}
				//ģʽ�������openmv����			
				Xianfu_Pwm(5000);  //===PWMƵ���޷�
				Set_Pwm(Motor_A,Motor_B,Motor_C);    //��ֵ��PWM�Ĵ���
		 }
	 }
 }
	 return 0;	 
} 


/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int motor_c)
{
			int Final_Motor_A,Final_Motor_B,Final_Motor_C;//���յĵ��ת��
	    static long int Last_Target_A,Last_Target_B,Last_Target_C;
	    static int flag,count;
	   	if(motor_a>0)			    INA=0;   //���A������ƣ�0Ϊ��ת��1Ϊ��ת
			else 	             	  INA=1;
	   	if(motor_b>0)			    INB=0;   //���B�������
			else 	             	  INB=1;
			if(motor_c>0)			    INC=0;   //���C�������
			else 	                INC=1;
	    if(Run_Flag==0)//�ٶ�ģʽ�µ��Զ�ʧ�ܵ�����
			{
					if(flag==0)
					{
							if(0==Target_A&&0==Target_B&&0==Target_C)  count++;
							else count=0;
							if(count>200)
							{
								flag=1;
								count=0;
							}
							ST	=1;//ʹ�ܵ��
					}
					if(flag==1)
					 {
								ST	=0;//���ʧ��
								if(Target_A!=0||Target_B!=0||Target_C!=0)
								flag=0;	
						}
			}
			else//λ��ģʽ�µ��Զ�ʧ�ܵ�����
		  {
					if(flag==0)
					{								
								if(Motor_A==0&&Motor_B==0&&Motor_C==0)   count++;
						  else count=0;
							if(count>200)
//							if(count>100)
							{
								Position_Move_Line_stop_flag=0;
								flag=1;
								count=0;
							}
							ST	=1;//ʹ�ܵ��
					}
					if(flag==1) //ͣ��״̬
					{
							ST	=0;//���ʧ��
							if(myabs(Last_Target_A-Target_A)>5||myabs(Last_Target_B-Target_B)>5||myabs(Last_Target_C-Target_C)>5) //Ŀ��ֵ����������5��ʹ�ܵ��
							flag=0;	
					}
			}
      Last_Target_A=Target_A;  //������ʷĿ��ֵ��Ϣ
			Last_Target_B=Target_B;
			Last_Target_C=Target_C;
	    Final_Motor_A=Linear_Conversion(motor_a);  //���Ի�
    	Final_Motor_B=Linear_Conversion(motor_b);
			Final_Motor_C=Linear_Conversion(motor_c);
			Set_PWM_Final(Final_Motor_A,Final_Motor_B,Final_Motor_C);  
}
/**************************************************************************
�������ܣ��Կ��������PWM���Ի�,���ڸ�ϵͳ�Ĵ�����ֵ
��ڲ�����PWM
����  ֵ�����Ի����PWM
**************************************************************************/
u16  Linear_Conversion(int motor)
{ 
	 u32 temp;
   u16 Linear_Moto;
   temp=1000000/myabs(motor);   //1000000�Ǿ���ֵ
	 if(temp>65535) Linear_Moto=65535;
	 else Linear_Moto=(u16)temp;
	 return Linear_Moto;
}	

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������ֵ
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(int amplitude)//amplitude�޷�
{	
    if(Motor_A<-amplitude) Motor_A=-amplitude;	
		if(Motor_A>amplitude)  Motor_A=amplitude;	
	  if(Motor_B<-amplitude) Motor_B=-amplitude;	
		if(Motor_B>amplitude)  Motor_B=amplitude;		
	  if(Motor_C<-amplitude) Motor_C=-amplitude;	
		if(Motor_C>amplitude)  Motor_C=amplitude;		
}
/**************************************************************************
�������ܣ�λ��PID���ƹ������ٶȵ�����
��ڲ������ޡ���ֵ
����  ֵ����
**************************************************************************/
void Xianfu_Velocity(int amplitude_A,int amplitude_B,int amplitude_C)
{	
    if(Motor_A<-amplitude_A) Motor_A=-amplitude_A;	//λ�ÿ���ģʽ�У�A����������ٶ�
		if(Motor_A>amplitude_A)  Motor_A=amplitude_A;	  //λ�ÿ���ģʽ�У�A����������ٶ�
	  if(Motor_B<-amplitude_B) Motor_B=-amplitude_B;	//λ�ÿ���ģʽ�У�B����������ٶ�
		if(Motor_B>amplitude_B)  Motor_B=amplitude_B;		//λ�ÿ���ģʽ�У�B����������ٶ�
	  if(Motor_C<-amplitude_C) Motor_C=-amplitude_C;	//λ�ÿ���ģʽ�У�C����������ٶ�
		if(Motor_C>amplitude_C)  Motor_C=amplitude_C;		//λ�ÿ���ģʽ�У�C����������ٶ�
}
/**************************************************************************
�������ܣ������޸�С������״̬ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Key(void)
{	
	u8 tmp;
	tmp=click_N_Double(100);    
	if(tmp==2)Flag_Show=!Flag_Show;//˫��������ʾģʽ                  
}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
u8 Turn_Off( int voltage)
{
	    u8 temp;
//			if(voltage<1110)//��ص�ѹ����11.1V�رյ��
	    if(voltage<950)//��ص�ѹ����11.1V�رյ��
			{	                                                
      temp=1;      
      PWMA=0;
      PWMB=0;
      PWMC=0;
      INA=0;
      INB=0;
      INC=0;	
			ST=0;   //ʧ�ܵ��
      }
			else
      temp=0;
      return temp;			
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����long int
����  ֵ��unsigned int
**************************************************************************/
u32 myabs(long int a)
{ 		   
	  u32 temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
**************************************************************************/
int Position_PID_A (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
int Position_PID_B (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
int Position_PID_C (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
/**************************************************************************
�������ܣ�ͨ������ָ���С������ң��
��ڲ���������ָ��
����  ֵ����
**************************************************************************/
void Get_RC(u8 mode)
{

	float step=10;  //�����ٶȿ��Ʋ���ֵ��
	  if(mode==0)//�ٶ�
		  {	
				 if(Openmv_Control_flag==1){Openmv_Control();}
				 switch(Flag_Direction)   //�������,Y����ǰ��X�������ң�Z����ԭ����ת
				 {
				 case 1:  Move_X=0;           Move_Y+=step;                 break;//ǰ��
				 case 2:  Move_X+=step;       Move_Y+=step;                 break;//������
				 case 3:  Move_X+=step;       Move_Y=0;                     break;//����
				 case 4:  Move_X+=step;       Move_Y-=step;                 break;//������
				 case 5:  Move_X=0;           Move_Y-=step;                 break;//����
				 case 6:  Move_X-=step;       Move_Y-=step;                 break;//������
				 case 7:  Move_X-=step;       Move_Y=0;                     break;//����
				 case 8:  Move_X-=step;       Move_Y+=step;                 break;//����
				 case 9:  Move_Z-=step,Gyro_K=0;														break;//������   
         case 10: Move_Z+=step,Gyro_K=0;														break;//������	
         case 11: UART_DM_ReportData(DM_Action0);										break;//ң��ץ��
         case 12:	UART_DM_ReportData(DM_Action1);										break;//ң�ط���	 
				 default: Move_X=Move_X/1.1;	Move_Y=Move_Y/1.1; Move_Z=0,Gyro_K=0.004;	      break;	 
			
				}
				if(Move_X<-RC_Velocity) Move_X=-RC_Velocity;	   //�ٶȿ����޷���1600
				if(Move_X>RC_Velocity)  Move_X=RC_Velocity;	     
				if(Move_Y<-RC_Velocity) Move_Y=-RC_Velocity;	
				if(Move_Y>RC_Velocity)  Move_Y=RC_Velocity;	 
				if(Move_Z<-RC_Velocity) Move_Z=-RC_Velocity;	
				if(Move_Z>RC_Velocity)  Move_Z=RC_Velocity;	 
			 
	   }
	 else if(mode==1)//λ��ģʽ
		 {	
			 if(Position_Move_Line_flag==1){Position_Move_Line();}
			 //delay_ms(1000);
			 //if(f){Position_Move_Line();}
			 //if(Position_Move_Line_flag==1){if(f){Position_Move_Line();}}
				 switch(Flag_Direction)   //�������
				 {
				 case 1:  Move_Y+=RC_Position; Flag_Change=1;												break;//Flag_Changeû�����ϣ�����
				 case 2:  Move_X+=RC_Position; Flag_Change=2; Move_Y+=RC_Position;  break;
				 case 3:  Move_X+=RC_Position; Flag_Change=3;												break;
				 case 4:  Move_X+=RC_Position; Flag_Change=4;Move_Y-=RC_Position;		break;
				 case 5:  Move_Y-=RC_Position; Flag_Change=5;												break;
				 case 6:  Move_X-=RC_Position; Flag_Change=6;Move_Y-=RC_Position; 	break;
				 case 7:  Move_X-=RC_Position; Flag_Change=7;												break;
				 case 8:  Move_X-=RC_Position; Flag_Change=8;Move_Y+=RC_Position;		break;			 
				 case 9:  Move_Z-=RC_Position; Flag_Change=9; 											break;
				 case 10: Move_Z+=RC_Position; Flag_Change=10;											break;
//				 case 11:	Position_Move_Line();																			break;		 
				 default: break;	 
			  }
	  }
		 Kinematic_Analysis(Move_X,Move_Y,Move_Z);//�õ�����Ŀ��ֵ�������˶�ѧ����
}
/**************************************************************************
�������ܣ�ÿ�����λ�ÿ��ƹ����ٶȼ���
��ڲ�������
����  ֵ����
**************************************************************************/
void Count_Velocity(void)
{
	static double Last_Target_X,Last_Target_Y,Last_Target_Z,Divider;//Divider������
	double Bias_X,Bias_Y,Bias_Z;
	Bias_X=(Move_X-Last_Target_X);  //��X��λ����
	Bias_Y=(Move_Y-Last_Target_Y);	//��Y��λ����
	Bias_Z=(Move_Z-Last_Target_Z);	//��Z��λ����
	if(Bias_X!=0||Bias_Y!=0||Bias_Z!=0)Divider=sqrt(Bias_X*Bias_X+Bias_Y*Bias_Y+Bias_Z*Bias_Z);//sqrt�����ź���
	if(Bias_X!=0||Bias_Y!=0||Bias_Z!=0) Kinematic_Analysis2(Bias_X,Bias_Y,Bias_Z);//Kinematic_Analysis2�˶�ѧ����

	Xianfu_Velocity(Max_Velocity*myabs(Rate_A)/Divider,Max_Velocity*myabs(Rate_B)/Divider,Max_Velocity*myabs(Rate_C)/Divider); 

	Last_Target_X=Move_X;   //����X����һ�ε�λ����Ϣ�����ڵ���
	Last_Target_Y=Move_Y;   //����Y����һ�ε�λ����Ϣ�����ڵ���
	Last_Target_Z=Move_Z;   //����Z����һ�ε�λ����Ϣ�����ڵ���
}
/**************************************************************************
�������ܣ�����CAN���ߴ��ڿ���ָ����д���
��ڲ�������
����  ֵ����
**************************************************************************/
//void CAN_N_Usart_Control(void)
//{
//  int flag_X, flag_Y,flag_Z;
//	 if((rxbuf[7]&0x04)==0)flag_X=1;  else flag_X=-1;  //�������λ
//	 if((rxbuf[7]&0x02)==0)flag_Y=1;  else flag_Y=-1;  //�������λ
//	 if((rxbuf[7]&0x01)==0)flag_Z=1;  else flag_Z=-1;  //�������λ
//	 Move_X=flag_X*(rxbuf[1]*256+rxbuf[2]);
//	 Move_Y=flag_Y*(rxbuf[3]*256+rxbuf[4]);	
//	 Move_Z=flag_Z*(rxbuf[5]*256+rxbuf[6]);	
//	
//   if(rxbuf[0]==1)Kinematic_Analysis(Move_X,Move_Y,Move_Z),Gyro_K=0;    //�����˶�ѧ����
//	 if(rxbuf[0]==2)Target_A=Move_X,Target_B=Move_Y,Target_C=Move_Z;      //������ÿ��������п���
//}

/**************************************************************************
�������ܣ����趨·�߽���λ��
��ڲ�������
����  ֵ����
**************************************************************************/
void Position_Move_Line(void)
{

	if(Position_Move_Line_stop_flag==0)
	{
		  //Flag_Direction1=click_RC();
			if(erwei_flag==12 || erwei_flag== 22)
			{ 
					if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==0)Position_Move_Line_count+=1;		
			else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==1){Position_Move_Line_count+=1;Position_Move_Line_stop_flag=1;}
			else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==2)Position_Move_Line_count+=1;
		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==3)Position_Move_Line_count+=1;
		  else if(Position_A>=17400&&Position_A<=17600&&Position_Move_Line_count==4)
				{	Position_Move_Line_count+=1;Position_Move_Line_stop_flag=1;
     			printf("2");
			}
			else if(Position_A>=17400&&Position_A<=17600&&Position_Move_Line_count==5)Position_Move_Line_count+=1;
		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==6)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==2)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==3)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==4)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==5)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==6)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==7)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==8)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==9)Position_Move_Line_count+=1;			
			else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==7){Position_Move_Line_count=0;Position_Move_Line_flag=0;f=0;Flag_Direction=0;}			
//			else if(Position_A>=9900&&Position_A<=10100&&Position_Move_Line_count==3)Position_Move_Line_count+=1;
//			else if(Position_A>=-15100&&Position_A<=-14900&&Position_Move_Line_count==4)Position_Move_Line_count+=1;

//			else if(Position_A>=9900&&Position_A<=10100&&Position_Move_Line_count==5)Position_Move_Line_count+=1;
//			
//			else if(Position_A>=9900&&Position_A<=10100&&Position_Move_Line_count==6)Position_Move_Line_count+=1;
//			
//			else if(Position_A>=4900&&Position_A<=5100&&Position_Move_Line_count==7)Position_Move_Line_count+=1;
//                 		else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==8){Position_Move_Line_count=0;Position_Move_Line_flag=0;}
			
			 switch(Position_Move_Line_count)   //�������
				 {
				 case 1:  Move_Y+=5.3*RC_Position; Position_Move_Line_stop_flag=1;	break;
				 case 2:  Move_X=Move_X; Position_Move_Line_stop_flag=1;	break;
				 case 3:  Move_Y+=1.8*RC_Position; Position_Move_Line_stop_flag=1;	break;				 
				 case 4:  Move_X+=3.5*RC_Position; Position_Move_Line_stop_flag=1;	break;		 
				 case 5:  Move_X=Move_X; Position_Move_Line_stop_flag=1;	break;				 
				 case 6:  Move_X-=3.5*RC_Position; Position_Move_Line_stop_flag=1;	break;
				 case 7:  Move_Y-=7.1*RC_Position; Position_Move_Line_stop_flag=1;	break;
//				 case 3:  Move_Y+=RC_Position; Position_Move_Line_stop_flag=1;	break;
//				 case 4:  Move_Y+=RC_Position; Position_Move_Line_stop_flag=1;	break;
//				 case 5:  Move_Y+=RC_Position; Position_Move_Line_stop_flag=1;	break;
//				 case 6:  Move_Y-=RC_Position; Position_Move_Line_stop_flag=1;  break;
//				 case 7:  Move_Y-=RC_Position; Position_Move_Line_stop_flag=1;  break;
//				 case 8:  Move_Y-=RC_Position; Position_Move_Line_stop_flag=1;  break;
//				 case 9:  Move_Y-=RC_Position; Position_Move_Line_stop_flag=1;  break;
//				 case 10:  Move_Y-=RC_Position; Position_Move_Line_stop_flag=1;  break;	 
//				 case 3:   Move_Y+=6*RC_Position; Position_Move_Line_stop_flag=1; 	break;
//				 case 4:  Move_X-=5*RC_Position;  Position_Move_Line_stop_flag=1;		break;
//         case 5:  Move_X+=5*RC_Position; Move_Y+=RC_Position; Position_Move_Line_stop_flag=1; 	break;
//				 case 6:  Move_Y-=6*RC_Position; Position_Move_Line_stop_flag=1;		break;
//         case 7:  Move_X-=RC_Position; Position_Move_Line_stop_flag=1; 	break;
//				 case 8:  Move_X-=RC_Position; Position_Move_Line_stop_flag=1;		break;                                                              
				 default: break;	
				 }
				 
			}
		  else if(erwei_flag==11 || erwei_flag== 21 )
			{
			if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==0)Position_Move_Line_count+=1;		
			else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==1){Position_Move_Line_count+=1;Position_Move_Line_stop_flag=1;}
			else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==2)Position_Move_Line_count+=1;
		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==3)Position_Move_Line_count+=1;
		  //else if(Position_A>=-17600&&Position_A<=-17400&&Position_Move_Line_count==4)Position_Move_Line_count+=1;
			else if(Position_A>=-17600&&Position_A<=-17400&&Position_Move_Line_count==4){
				Position_Move_Line_count+=1;Position_Move_Line_stop_flag=1;
				printf("1");
			}
			else if(Position_A>=-17600&&Position_A<=-17400&&Position_Move_Line_count==5)Position_Move_Line_count+=1;
		  else if(Position_A>=-100&&Position_A<=100&&Position_Move_Line_count==6)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==2)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==3)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==4)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==5)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==6)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==7)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==8)Position_Move_Line_count+=1;
//		  else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==9)Position_Move_Line_count+=1;
			
			else if(Position_A>=-100&&Position_A<=100&&Position_Move_Line_count==7){Position_Move_Line_count=0;Position_Move_Line_flag=0;f=0;Flag_Direction=0;}
			
//			else if(Position_A>=9900&&Position_A<=10100&&Position_Move_Line_count==3)Position_Move_Line_count+=1;
//			else if(Position_A>=-15100&&Position_A<=-14900&&Position_Move_Line_count==4)Position_Move_Line_count+=1;

//			else if(Position_A>=9900&&Position_A<=10100&&Position_Move_Line_count==5)Position_Move_Line_count+=1;
//			
//			else if(Position_A>=9900&&Position_A<=10100&&Position_Move_Line_count==6)Position_Move_Line_count+=1;
//			
//			else if(Position_A>=4900&&Position_A<=5100&&Position_Move_Line_count==7)Position_Move_Line_count+=1;
//                 		else if(Position_A>=0&&Position_A<=100&&Position_Move_Line_count==8){Position_Move_Line_count=0;Position_Move_Line_flag=0;}
			
			 switch(Position_Move_Line_count)   //�������
				 {
				 case 1:  Move_Y+=5.3*RC_Position; Position_Move_Line_stop_flag=1;	break;
				 case 2:  Move_X=Move_X; Position_Move_Line_stop_flag=1;	break;
				 case 3:  Move_Y+=1.8*RC_Position; Position_Move_Line_stop_flag=1;	break;				 
				 case 4:  Move_X-=3.5*RC_Position; Position_Move_Line_stop_flag=1;	break;					 
				 case 5:  Move_X=Move_X; Position_Move_Line_stop_flag=1;	break;	 		 
				 case 6:  Move_X+=3.5*RC_Position; Position_Move_Line_stop_flag=1;	break;
				 case 7:  Move_Y-=7.1*RC_Position; Position_Move_Line_stop_flag=1;	break;
//				 case 3:  Move_Y+=RC_Position; Position_Move_Line_stop_flag=1;	break;
//				 case 4:  Move_Y+=RC_Position; Position_Move_Line_stop_flag=1;	break;
//				 case 5:  Move_Y+=RC_Position; Position_Move_Line_stop_flag=1;	break;
//				 case 6:  Move_Y-=RC_Position; Position_Move_Line_stop_flag=1;  break;
//				 case 7:  Move_Y-=RC_Position; Position_Move_Line_stop_flag=1;  break;
//				 case 8:  Move_Y-=RC_Position; Position_Move_Line_stop_flag=1;  break;
//				 case 9:  Move_Y-=RC_Position; Position_Move_Line_stop_flag=1;  break;
//				 case 10:  Move_Y-=RC_Position; Position_Move_Line_stop_flag=1;  break;	 
//				 case 3:   Move_Y+=6*RC_Position; Position_Move_Line_stop_flag=1; 	break;
//				 case 4:  Move_X-=5*RC_Position;  Position_Move_Line_stop_flag=1;		break;
//         case 5:  Move_X+=5*RC_Position; Move_Y+=RC_Position; Position_Move_Line_stop_flag=1; 	break;
//				 case 6:  Move_Y-=6*RC_Position; Position_Move_Line_stop_flag=1;		break;
//         case 7:  Move_X-=RC_Position; Position_Move_Line_stop_flag=1; 	break;
//				 case 8:  Move_X-=RC_Position; Position_Move_Line_stop_flag=1;		break;                                                               
				 default: break;	
				 }
			 }
				 

	 }
}
void Openmv_Control(void)
{
		if(speed_y<=100)Move_Y=speed_y*16;
		if(speed_y>100&&speed_y<=300)Move_Y=speed_y*8;
//		Move_X=speed_x; 		
//	  Move_Z=speed_z,Gyro_K=0;													
    if(speed_z<=100)Move_Z=speed_z*16,Gyro_K=0;
		if(speed_z>100&&speed_z<=300)Move_Z=speed_z*8,Gyro_K=0;
}

