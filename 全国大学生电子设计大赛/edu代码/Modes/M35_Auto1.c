#include "Modes.h"
#include "Basic.h"
#include "stdlib.h"
#include <stdio.h>
#include "M35_Auto1.h"

#include "AC_Math.h"
#include "Receiver.h"
#include "InteractiveInterface.h"
#include "ControlSystem.h"
#include "MeasurementSystem.h"
#include "drv_Uart2.h"

static void M35_Auto1_MainFunc();
static void M35_Auto1_enter();
static void M35_Auto1_exit();
const Mode M35_Auto1 = 
{
	50 , //mode frequency
	M35_Auto1_enter , //enter
	M35_Auto1_exit ,	//exit
	M35_Auto1_MainFunc ,	//mode main func
};

typedef struct
{
	//�˳�ģʽ������
	uint16_t exit_mode_counter;
	
	//�Զ�����״̬��
	uint8_t auto_step1;	//0-��¼��ťλ��
											//1-�ȴ���ť������� 
											//2-�ȴ������� 
											//3-�ȴ�2��
											//4-����
											//5-�ȴ��������
	uint16_t auto_counter;
	float last_button_value;
	
	float last_height;
}MODE_INF;
static MODE_INF* Mode_Inf;

static void M35_Auto1_enter()
{
	Led_setStatus( LED_status_running1 );
	
	//��ʼ��ģʽ����
	Mode_Inf = malloc( sizeof( MODE_INF ) );
	Mode_Inf->exit_mode_counter = 0;
	Mode_Inf->auto_step1 = Mode_Inf->auto_counter = 0;
	Altitude_Control_Enable();
}

static void M35_Auto1_exit()
{
	Altitude_Control_Disable();
	Attitude_Control_Disable();
	
	free( Mode_Inf );
}

static void M35_Auto1_MainFunc()
{
	const Receiver* rc = get_current_Receiver();
		
	if( rc->available == false )
	{
		//���ջ�������
		//����
		Position_Control_set_XYLock();
		Position_Control_set_TargetVelocityZ( -50 );
		return;
	}
	float throttle_stick = rc->data[0];
	float yaw_stick = rc->data[1];
	float pitch_stick = rc->data[2];
	float roll_stick = rc->data[3];	
	
	/*�ж��˳�ģʽ*/
		if( throttle_stick < 5 && yaw_stick < 5 && pitch_stick < 5 && roll_stick > 95 )
		{
			if( ++Mode_Inf->exit_mode_counter >= 50 )
			{
				change_Mode( 1 );
				return;
			}
		}
		else
			Mode_Inf->exit_mode_counter = 0;
	/*�ж��˳�ģʽ*/
		
	//�ж�ҡ���Ƿ����м�
	bool sticks_in_neutral = 
		in_symmetry_range_offset_float( throttle_stick , 5 , 50 ) && \
		in_symmetry_range_offset_float( yaw_stick , 5 , 50 ) && \
		in_symmetry_range_offset_float( pitch_stick , 5 , 50 ) && \
		in_symmetry_range_offset_float( roll_stick , 5 , 50 );
	
	extern float SDI_Point[6];
	extern TIME SDI_Time;
	extern int32_t SDI2_Point[20];
	
	if( sticks_in_neutral && get_Position_Measurement_System_Status() == Measurement_System_Status_Ready )
	{
		//ҡ�����м�
		//ִ���Զ�����		
		//ֻ����λ����Чʱ��ִ���Զ�����
		
		//��ˮƽλ�ÿ���
		#define write_new_byte(buffer,i,byte) buffer[i++]=byte;CK_A+=byte; CK_B+=CK_A
		Position_Control_Enable();
		extern TIME SDI2_Time;
		switch( Mode_Inf->auto_step1 )
		{
			case 0:
				Mode_Inf->last_button_value = rc->data[5];
				++Mode_Inf->auto_step1;
				Mode_Inf->auto_counter = 0;
				break;
			
			case 1:
				//�ȴ���ť�������
				if( get_is_inFlight() == false && fabsf( rc->data[5] - Mode_Inf->last_button_value ) > 15 )
				{
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
				}
				else
					goto ManualControl;
				break;
				
			case 2:	
			{				
				uint8_t buf[30];
				unsigned char CK_A = 0 , CK_B = 0;
				buf[0] = 'A';	buf[1] = 'C';
				int buffer_index = 2;
				write_new_byte( buf, buffer_index, 201 );
				write_new_byte( buf, buffer_index, 0 );
				buf[ buffer_index++ ] = CK_A;
				buf[ buffer_index++ ] = CK_B;
				Uart2_Send( buf, buffer_index );
				if( get_pass_time(SDI2_Time)<0.5 && SDI2_Point[0]==210 )
				{
					Position_Control_Takeoff_HeightRelative( 300.0f );
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
				}
				break;
			}
			case 3:
				//�ȴ������ɷ�ֱ��
				if( get_Altitude_ControlMode() == Position_ControlMode_Position )
				{
					Position_Control_set_TargetPositionZRelative( 1700 );
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
				}
				break;
				
			case 4:
				//�ȴ������ɷ�ֱ��
				if( get_Altitude_ControlMode() == Position_ControlMode_Position )
				{
					uint8_t buf[30];
					unsigned char CK_A = 0 , CK_B = 0;
					buf[0] = 'A';	buf[1] = 'C';
					int buffer_index = 2;
					write_new_byte( buf, buffer_index, 202 );
					write_new_byte( buf, buffer_index, 0 );
					buf[ buffer_index++ ] = CK_A;
					buf[ buffer_index++ ] = CK_B;
					Uart2_Send( buf, buffer_index );
					
					Position_Control_set_TargetPositionXY_LatLon( SDI2_Point[2]*1e-7, SDI2_Point[1]*1e-7 );
					//Position_Control_set_TargetPositionXY_LatLon( 23.154063 , 113.028636 );
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
				}
				break;
				
			case 5:
				//�ȴ�ֱ�߷�������½�
				if( get_Position_ControlMode() == Position_ControlMode_Position )
				{
					uint8_t buf[30];
					unsigned char CK_A = 0 , CK_B = 0;
					buf[0] = 'A';	buf[1] = 'C';
					int buffer_index = 2;
					write_new_byte( buf, buffer_index, 203 );
					write_new_byte( buf, buffer_index, 0 );
					buf[ buffer_index++ ] = CK_A;
					buf[ buffer_index++ ] = CK_B;
					Uart2_Send( buf, buffer_index );
					
					Position_Control_set_TargetPositionZRelative( -1500 );
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
					Mode_Inf->last_height = 500;
					OLED_Draw_TickCross8x6( false , 0 , 0 );
				}
				break;
				
			case 6:
				//�ȴ����߶����
				if( get_Altitude_ControlMode() == Position_ControlMode_Position )
				{
					uint8_t buf[30];
					unsigned char CK_A = 0 , CK_B = 0;
					buf[0] = 'A';	buf[1] = 'C';
					int buffer_index = 2;
					write_new_byte( buf, buffer_index, 204 );
					write_new_byte( buf, buffer_index, 0 );
					buf[ buffer_index++ ] = CK_A;
					buf[ buffer_index++ ] = CK_B;
					Uart2_Send( buf, buffer_index );
					
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
				}
				break;
				
			case 7:
				if( Time_isValid(SDI_Time) && get_pass_time(SDI_Time) < 1.0f )
				{
					Position_Control_set_TargetPositionXYRelativeBodyHeading( SDI_Point[0] , SDI_Point[1] );
					Position_Control_set_TargetPositionZRelative( -(SDI_Point[2] - 300) );
					
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
				}
				break;
			
			case 8:
				if( get_Position_ControlMode() == Position_ControlMode_Position && get_Altitude_ControlMode() == Position_ControlMode_Position )
				{
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
				}
				break;
				
			case 9:
				if( Time_isValid(SDI_Time) && get_pass_time(SDI_Time) < 1.0f )
				{
					Attitude_Control_set_Target_YawRelative( SDI_Point[3] + PI_f/2 );
					
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
				}
				break;
			
			case 10:
				if( ++Mode_Inf->auto_counter == 200 )
				{
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
				}
				break;
				
			case 11:			
				OLED_Draw_Str8x6( "     " , 2 , 0 );
				OLED_Draw_Str8x6( "     " , 3 , 0 );
				if( Time_isValid(SDI_Time) && get_pass_time(SDI_Time) < 1.0f )
				{
					OLED_Draw_TickCross8x6( true , 0 , 0 );
					char str[5];
					sprintf( str , "%5.1f", SDI_Point[0] );
					OLED_Draw_Str8x6( str , 2 , 0 );
					sprintf( str , "%5.1f" , SDI_Point[1] );
					OLED_Draw_Str8x6( str , 3 , 0 );
					Position_Control_set_TargetVelocityBodyHeadingXY_AngleLimit( \
						constrain_float( SDI_Point[0] * 1.2f , 50 ) ,	\
						constrain_float( SDI_Point[1] * 1.2f , 50 ) ,	\
						0.15f ,	\
						0.15f	\
					);
					Position_Control_set_TargetVelocityZ(	\
						constrain_range_float( -SDI_Point[2] * 0.1f , -20 , -50 )	\
					);
					//Attitude_Control_set_Target_YawRate( constrain_float( SDI_Point[3] * 0.2f , 0.6f ) );
					Mode_Inf->last_height = SDI_Point[2];
				}
				else
				{
					//Ŀ�겻���ã�ɲ����λ��
					Position_Control_set_XYLock();
					if( Mode_Inf->last_height > 100 )
						Position_Control_set_ZLock();
					else
						Position_Control_set_TargetVelocityZ( -30 );
				//	Attitude_Control_set_YawLock();
				}
				OLED_Update();
				
				//�ȴ���ť����
				if( get_is_inFlight() == false )
				{	
					Position_Control_set_XYLock();
					Mode_Inf->auto_step1 = 12;
					Mode_Inf->auto_counter = 0;
				}
				break;
				
			case 12:
			{
				uint8_t buf[30];
				unsigned char CK_A = 0 , CK_B = 0;
				buf[0] = 'A';	buf[1] = 'C';
				int buffer_index = 2;
				write_new_byte( buf, buffer_index, 205 );
				write_new_byte( buf, buffer_index, 0 );
				buf[ buffer_index++ ] = CK_A;
				buf[ buffer_index++ ] = CK_B;
				Uart2_Send( buf, buffer_index );
				if( ++Mode_Inf->auto_counter >= 1000 )
				{
					Mode_Inf->auto_step1 = 0;
					Mode_Inf->auto_counter = 0;
				}
				else if( get_pass_time(SDI2_Time)<0.5 && SDI2_Point[0]==211 )
				{
					Mode_Inf->auto_step1 = 13;
					Mode_Inf->auto_counter = 0;
				}
				break;
			}		
			
			case 13:
			{
				uint8_t buf[30];
				unsigned char CK_A = 0 , CK_B = 0;
				buf[0] = 'A';	buf[1] = 'C';
				int buffer_index = 2;
				write_new_byte( buf, buffer_index, 206 );
				write_new_byte( buf, buffer_index, 0 );
				buf[ buffer_index++ ] = CK_A;
				buf[ buffer_index++ ] = CK_B;
				Uart2_Send( buf, buffer_index );
				if( get_pass_time(SDI2_Time)<0.5 && SDI2_Point[0]==210 )
				{
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
				}
				break;
			}

			case 14:
			{
				Position_Control_Takeoff_HeightRelative( 300.0f );
				++Mode_Inf->auto_step1;
				Mode_Inf->auto_counter = 0;
			}
			
			case 15:
			{
				//�ȴ������ɷ�ֱ��
				if( get_Altitude_ControlMode() == Position_ControlMode_Position )
				{
					Mode_Inf->auto_step1 = 9;
					Mode_Inf->auto_counter = 0;
				}
				break;
			}
		}
		
			
	}
	else
	{
		ManualControl:
		//ҡ�˲����м�
		//�ֶ�����
		Mode_Inf->auto_step1 = Mode_Inf->auto_counter = 0;
		
		char str[16];
		double Lat , Lon;
		get_LatLon_From_Point( get_Position().x , get_Position().y , &Lat , &Lon );
		sprintf( str , "%d", (int)(Lat*1e7) );
		OLED_Draw_Str8x6( str , 0 , 0 );
		sprintf( str , "%d", (int)(Lon*1e7) );
		OLED_Draw_Str8x6( str , 1 , 0 );
		OLED_Update();
		
		//�ر�ˮƽλ�ÿ���
		Position_Control_Disable();
		
		//�߶ȿ�������
		if( in_symmetry_range_offset_float( throttle_stick , 5 , 50 ) )
			Position_Control_set_ZLock();
		else
			Position_Control_set_TargetVelocityZ( ( throttle_stick - 50.0f ) * 6 );

		//ƫ����������
		if( in_symmetry_range_offset_float( yaw_stick , 5 , 50 ) )
			Attitude_Control_set_YawLock();
		else
			Attitude_Control_set_Target_YawRate( ( 50.0f - yaw_stick )*0.05f );
		
		//Roll Pitch��������
		Attitude_Control_set_Target_RollPitch( \
			( roll_stick 	- 50.0f )*0.015f, \
			( pitch_stick - 50.0f )*0.015f );
	}
}