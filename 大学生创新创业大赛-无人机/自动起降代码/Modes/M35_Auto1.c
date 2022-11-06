#include "Modes.h"
#include "Basic.h"
#include "stdlib.h"
#include "M35_Auto1.h"

#include "AC_Math.h"
#include "Receiver.h"
#include "InteractiveInterface.h"
#include "ControlSystem.h"
#include "MeasurementSystem.h"

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
	//退出模式计数器
	uint16_t exit_mode_counter;
	
	//自动飞行状态机
	uint8_t auto_step1;	

	uint16_t auto_counter;
	float last_button_value;
}MODE_INF;
static MODE_INF* Mode_Inf;

static void M35_Auto1_enter()
{
	Led_setStatus( LED_status_running1 );
	
	//初始化模式变量
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
		//接收机不可用
		//降落
		Position_Control_set_XYLock();
		Position_Control_set_TargetVelocityZ( -50 );
		return;
	}
	float throttle_stick = rc->data[0];
	float yaw_stick = rc->data[1];
	float pitch_stick = rc->data[2];
	float roll_stick = rc->data[3];	
	

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

		

	bool sticks_in_neutral = 
		in_symmetry_range_offset_float( throttle_stick , 5 , 50 ) && \
		in_symmetry_range_offset_float( yaw_stick , 5 , 50 ) && \
		in_symmetry_range_offset_float( pitch_stick , 5 , 50 ) && \
		in_symmetry_range_offset_float( roll_stick , 5 , 50 );
	
	
	
	if( sticks_in_neutral && get_Position_Measurement_System_Status() == Measurement_System_Status_Ready )
	{
		//摇杆在中间
		//执行自动飞行		
		//只有在位置有效时才执行自动飞行
		//打开水平位置控制
		Position_Control_Enable();
		switch( Mode_Inf->auto_step1 )
		{
			case 0:
				Mode_Inf->last_button_value = rc->data[5];
				++Mode_Inf->auto_step1;
				Mode_Inf->auto_counter = 0;
				break;
			
			case 1:
				//按钮按下起飞
				if( get_is_inFlight() == false && fabsf( rc->data[5] - Mode_Inf->last_button_value ) > 15 )
				{
					Position_Control_Takeoff_HeightRelative( 100.0f );
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
				}
				break;
				
			case 2:
				//等起飞完成
				if( get_Altitude_ControlMode() == Position_ControlMode_Position )
				{
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
				}
				break;
				
			case 3:
				//等2秒
				if( ++Mode_Inf->auto_counter == 100 )
				{
					++Mode_Inf->auto_step1;
					Mode_Inf->auto_counter = 0;
				}
				break;
				
			case 4:
				//降
				Position_Control_set_TargetVelocityZ( -50 );
				++Mode_Inf->auto_step1;
				Mode_Inf->auto_counter = 0;
				break;
			
			case 5:
				//降完
				if( get_is_inFlight() == false )
				{
					Mode_Inf->auto_step1 = 0;
					Mode_Inf->auto_counter = 0;
				}
				break;
		}
	}
	else
	{

		Mode_Inf->auto_step1 = Mode_Inf->auto_counter = 0;
		
		//关水平位置控制
		Position_Control_Disable();
		
		//高度控制输入
		if( in_symmetry_range_offset_float( throttle_stick , 5 , 50 ) )
			Position_Control_set_ZLock();
		else
			Position_Control_set_TargetVelocityZ( ( throttle_stick - 50.0f ) * 6 );

		//偏航控制输入
		if( in_symmetry_range_offset_float( yaw_stick , 5 , 50 ) )
			Attitude_Control_set_YawLock();
		else
			Attitude_Control_set_Target_YawRate( ( 50.0f - yaw_stick )*0.05f );
		
		//Roll Pitch控制输入
		Attitude_Control_set_Target_RollPitch( \
			( roll_stick 	- 50.0f )*0.015f, \
			( pitch_stick - 50.0f )*0.015f );
	}
}