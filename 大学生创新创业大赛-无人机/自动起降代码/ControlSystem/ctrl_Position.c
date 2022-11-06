#include "Basic.h"
#include "ctrl_Position.h"

#include "ControlSystem.h"
#include "MeasurementSystem.h"
#include "Configurations.h"
#include "drv_PWMOut.h"
#include "Receiver.h"

#include "AC_Math.h"
#include "Quaternion.h"
#include "TD4.h"
#include "Filters_Butter.h"

/*模式及期望*/
	static bool Altitude_Control_Enabled = false;
	static bool Position_Control_Enabled = false;
	
	//位置控制模式
	static Position_ControlMode Altitude_ControlMode = Position_ControlMode_Position;
	static Position_ControlMode HorizontalPosition_ControlMode = Position_ControlMode_Position;
	
	//期望TD4滤波器
	static TD4 Target_tracker[3];
	//期望速度低通滤波器
	static Filter_Butter4_LP_float TargetVelocityFilter[3];
	
	static vector3_float target_position;
	static vector3_float target_velocity;
	
	/*高度*/
		bool Altitude_Control_Enable()
		{
			if( Altitude_Control_Enabled == true ||\
				get_Altitude_Measurement_System_Status() != Measurement_System_Status_Ready )
				return false;
			
			Altitude_ControlMode = Position_ControlMode_Locking;
			Altitude_Control_Enabled = true;
			Attitude_Control_Enable();
			return true;
		}
		bool Altitude_Control_Disable()
		{
			if( Altitude_Control_Enabled == false )
				return false;		
			Altitude_Control_Enabled = false;		
			Position_Control_Disable();
			return true;
		}
		
		Position_ControlMode get_Altitude_ControlMode()
		{
			return Altitude_ControlMode;
		}
		
		bool Position_Control_set_TargetPositionZ( float posz )
		{
			if( Altitude_Control_Enabled == false )
				return false;
			target_position.z = posz;
			if( Altitude_ControlMode != Position_ControlMode_Position )
			{
				Target_tracker[2].x1 = get_Position().z;
				Altitude_ControlMode = Position_ControlMode_Position;
			}
			return true;
		}
		bool Position_Control_set_TargetVelocityZ( float velz )
		{
			if( Altitude_Control_Enabled == false )
				return false;
			target_velocity.z = velz;
			Altitude_ControlMode = Position_ControlMode_Velocity;
			return true;
		}
		bool Position_Control_set_ZLock()
		{
			if( Altitude_Control_Enabled == false )
				return false;
			if( Altitude_ControlMode == Position_ControlMode_Velocity )
				Altitude_ControlMode = Position_ControlMode_Locking;
			return true;
		}
		
		//起飞到当前高度上方的height高度
		bool Position_Control_Takeoff_HeightRelative( float height )
		{
			if( Altitude_Control_Enabled == false )
				return false;
			if( height < 10 )
				return false;
			if( get_is_inFlight() == true )
				return false;
			
			target_position.z = get_Position().z + height;
			if( Altitude_ControlMode != Position_ControlMode_Takeoff )
			{
				Target_tracker[2].x1 = get_Position().z;
				Altitude_ControlMode = Position_ControlMode_Takeoff;
			}
			return true;
		}
	/*高度*/
		
	/*水平位置*/
		bool get_Position_Control_Enabled()
		{
			return Position_Control_Enabled;
		}
		bool Position_Control_Enable()
		{
			if( Position_Control_Enabled == true ||\
				get_Position_Measurement_System_Status() != Measurement_System_Status_Ready )
				return false;
				
			HorizontalPosition_ControlMode = Position_ControlMode_Locking;
			Position_Control_Enabled = true;
			Altitude_Control_Enable();
			return true;
		}
		bool Position_Control_Disable()
		{
			if( Position_Control_Enabled == false )
				return false;		
			Position_Control_Enabled = false;		
			return true;
		}
			
		Position_ControlMode get_Position_ControlMode()
		{
			return HorizontalPosition_ControlMode;
		}
		
		bool Position_Control_set_TargetPositionXY( float posx , float posy )
		{
			if( Position_Control_Enabled == false )
				return false;
			target_position.x = posx;
			target_position.y = posy;
			if( HorizontalPosition_ControlMode != Position_ControlMode_Position )
			{
				Target_tracker[0].x1 = get_Position().x;
				HorizontalPosition_ControlMode = Position_ControlMode_Position;
			}
			return true;
		}
		bool Position_Control_set_TargetVelocityBodyHeadingXY( float velx , float vely )
		{
			if( Position_Control_Enabled == false )
				return false;
			
			//将速度旋转至ENU
			float Yaw = Quaternion_getYaw( get_Airframe_attitude() );
			float Yaw_sin , Yaw_cos;
			arm_sin_cos_f32( rad2degree(Yaw) , &Yaw_sin , &Yaw_cos );
			float velx_ENU = map_BodyHeading2ENU_x( velx , vely , Yaw_sin , Yaw_cos );
			float vely_ENU = map_BodyHeading2ENU_y( velx , vely , Yaw_sin , Yaw_cos );
			
			target_velocity.x = velx_ENU;
			target_velocity.y = vely_ENU;
			HorizontalPosition_ControlMode = Position_ControlMode_Velocity;
			return true;
		}
		bool Position_Control_set_XYLock()
		{
			if( Position_Control_Enabled == false )
				return false;
			if( HorizontalPosition_ControlMode == Position_ControlMode_Velocity )
				HorizontalPosition_ControlMode = Position_ControlMode_Locking;
			return true;
		}
	/*水平位置*/
		
	/*自动控制*/
		
	/*自动控制*/	
		
/*模式及期望*/

void ctrl_Position()
{
	bool inFlight = get_is_inFlight();
	vector3_float Position = get_Position();
	vector3_float VelocityENU = get_VelocityENU();
	vector3_float AccelerationENU = get_AccelerationCtrl();
	
	float Ps = 1.0f;
	float Pv = 2.0f;
	if( Altitude_Control_Enabled )
	{//高度控制
			
		switch( Altitude_ControlMode )
		{
			case Position_ControlMode_Position:
			{	//控制位置
				if( inFlight )
				{
					Target_tracker[2].r2n = Target_tracker[2].r2p = 200;
					TD4_track4( &Target_tracker[2] , target_position.z , 1.0f / 200 );
				}
				else
				{
					//没起飞前在位置控制模式
					//不要起飞
					TD4_reset( &Target_tracker[2] );
					target_position.z = Target_tracker[2].x1 = Position.z;
					Attitude_Control_set_Throttle( 0 );
					goto AltCtrl_Finish;
				}
				break;
			}
			case Position_ControlMode_Velocity:
			{	//控制速度
				if( inFlight || target_velocity.z > 0 )
					TD4_track3( &Target_tracker[2] , target_velocity.z , 1.0f / 200 );
				else
				{
					//没起飞且期望速度为负
					//不要起飞
					TD4_reset( &Target_tracker[2] );
					Target_tracker[2].x1 = Position.z;
					Attitude_Control_set_Throttle( 0 );
					goto AltCtrl_Finish;
				}
				break;
			}
			case Position_ControlMode_Locking:
			{	//锁位置（减速到0然后锁住高度）
				if( inFlight )
				{
					TD4_track3( &Target_tracker[2] , 0 , 1.0f / 200 );
					if( in_symmetry_range_float( VelocityENU.z , 10.0f ) && in_symmetry_range_float( AccelerationENU.z , 50.0f ) && \
							in_symmetry_range_float( Target_tracker[2].x2 , 0.1f ) && \
							in_symmetry_range_float( Target_tracker[2].x3 , 0.1f )	)
					{
						target_position.z = Target_tracker[2].x1 = Position.z;
						Altitude_ControlMode = Position_ControlMode_Position;
					}
				}
				else
				{
					Attitude_Control_set_Throttle( 0 );
					goto AltCtrl_Finish;
				}
				break;
			}
			
			case Position_ControlMode_Takeoff:
			{	//起飞
				Target_tracker[2].r2n = Target_tracker[2].r2p = 200;
				TD4_track4( &Target_tracker[2] , target_position.z , 1.0f / 200 );
				if( fabsf( target_position.z - Position.z ) < 10 && \
						in_symmetry_range_float( VelocityENU.z , 10.0f ) &&  \
						in_symmetry_range_float( AccelerationENU.z , 50.0f ) && \
						in_symmetry_range_float( Target_tracker[2].x2 , 0.1f ) && \
						in_symmetry_range_float( Target_tracker[2].x3 , 0.1f )	)
					Altitude_ControlMode = Position_ControlMode_Position;
			}
		}
		
		//计算期望速度
		float target_velocity_z;
		//期望垂直速度的导数
		float Tvz_1;
		if( Target_tracker[2].tracking_mode == 4 )
		{
			target_velocity_z = Ps * ( Target_tracker[2].x1 - Position.z ) + Target_tracker[2].x2;
			Tvz_1 = Ps * ( Target_tracker[2].x2 - VelocityENU.z ) + Target_tracker[2].x3;
		}
		else
		{
			target_velocity_z = Target_tracker[2].x2;
			Tvz_1 = Target_tracker[2].x3;
		}
		
		//计算期望加速度
		float target_acceleration_z = Pv * ( target_velocity_z - VelocityENU.z ) + Tvz_1;
		target_acceleration_z = Filter_Butter4_LP_float_run( &TargetVelocityFilter[2] , target_acceleration_z );
		//加速度误差
		float acceleration_z_error = target_acceleration_z - AccelerationENU.z;
		
		//获取倾角cosin
		float lean_cosin = get_lean_angle_cosin();
		if( lean_cosin < 0.5f )
			lean_cosin = 0.5f;
		
		//获取电机起转油门
		float MotorStartThrottle = Cfg_get_MotorStartingThrottle();
		//获取悬停油门 - 电机起转油门
		float hover_throttle = get_hover_throttle() - MotorStartThrottle;
		
		//悬停油门时：
		//输出力 = mg
		//需获得Ta加速度时：
		//需输出的力为：( 1 + Ta/G )
		//计算输出油门 =    悬停油门 * ( 1 + Ta / G )  +   P*( Ta - a )
		float throttle = hover_throttle * ( 1.0f + target_acceleration_z / constG ) + 0.025f*acceleration_z_error;
		//倾角补偿
		throttle /= lean_cosin;
		
		//油门限幅
		throttle += MotorStartThrottle;
		if( throttle > 90 )
			throttle = 90;
		if( inFlight )
		{
			if( throttle < MotorStartThrottle )
				throttle = MotorStartThrottle;
		}
		
		//输出
		Attitude_Control_set_Throttle( throttle );
		
	}//高度控制
AltCtrl_Finish:
	
	if( Position_Control_Enabled )
	{	//水平位置控制
		if( get_Position_Measurement_System_Status() != Measurement_System_Status_Ready )
		{
			Position_Control_Enabled = false;
			goto PosCtrl_Finish;
		}
		
		//最大水平角度误差
		float max_angle_err_xy = 1.5f / 200;
		
		float Tvx_1 = 0 , Tvy_1 = 0;
		switch( HorizontalPosition_ControlMode )
		{
			case Position_ControlMode_Position:
			{
				if( inFlight )
				{
					target_velocity.x = Ps * ( target_position.x - Position.x );
					Tvx_1 = Ps * (  - VelocityENU.x );
					target_velocity.y = Ps * ( target_position.y - Position.y );
					Tvy_1 = Ps * (  - VelocityENU.y );
					
					max_angle_err_xy = 1.5f / 200;
				}
				else
				{
					//没起飞前在位置控制模式
					//重置期望位置
					target_position.x = Position.x;
					target_position.y = Position.y;
					return;
				}
				break;
			}		
			case Position_ControlMode_Velocity:
			{
				if( inFlight )
				{
					max_angle_err_xy = 3.5f / 200;
				}
				else
				{
					//没起飞时重置期望速度

				}
				break;
			}
			case Position_ControlMode_Locking:
			{
				if( inFlight )
				{
					max_angle_err_xy = 1.5f / 200;
					target_velocity.x = 0;
					target_velocity.y = 0;
					if( VelocityENU.x*VelocityENU.x + VelocityENU.y*VelocityENU.y < 10*10 )
					{
						target_position.x = Position.x;
						target_position.y = Position.y;
						HorizontalPosition_ControlMode = Position_ControlMode_Position;
					}
				}
				break;
			}
		}
			
		//计算期望加速度
		float target_acceleration_x = Pv * ( target_velocity.x - VelocityENU.x ) + Tvx_1;
		target_acceleration_x = Filter_Butter4_LP_float_run( &TargetVelocityFilter[0] , target_acceleration_x );
		float target_acceleration_y = Pv * ( target_velocity.y - VelocityENU.y ) + Tvy_1;
		target_acceleration_y = Filter_Butter4_LP_float_run( &TargetVelocityFilter[1] , target_acceleration_y );
		
		//去除风力扰动
		target_acceleration_x -= get_WindDisturbance_x();
		target_acceleration_y -= get_WindDisturbance_y();
		
		//旋转至Bodyheading
		float Yaw = Quaternion_getYaw( get_Airframe_attitude() );
		float Yaw_sin , Yaw_cos;
		arm_sin_cos_f32( rad2degree(Yaw) , &Yaw_sin , &Yaw_cos );
		float target_acceleration_x_bodyheading = map_ENU2BodyHeading_x( target_acceleration_x , target_acceleration_y , Yaw_sin , Yaw_cos );
		float target_acceleration_y_bodyheading = map_ENU2BodyHeading_y( target_acceleration_x , target_acceleration_y , Yaw_sin , Yaw_cos );
		
		float target_Roll = atan2f( -target_acceleration_y_bodyheading , constG );
		float target_Pitch = atan2f( target_acceleration_x_bodyheading , constG );
		float err_x = target_Roll - Attitude_Control_get_Target_Roll();
		float err_y = target_Pitch - Attitude_Control_get_Target_Pitch();
		constrain_vector2_float( &err_x , &err_y , max_angle_err_xy );
		target_Roll = Attitude_Control_get_Target_Roll() + err_x;
		target_Pitch = Attitude_Control_get_Target_Pitch() + err_y;
		
		Attitude_Control_set_Target_RollPitch( target_Roll, target_Pitch );
		
	}//水平位置控制
	
PosCtrl_Finish:
	return;
}

void init_ctrl_Position()
{
	//初始化期望TD4滤波器
	TD4_init( &Target_tracker[0] , 15 , 15 , 15 , 15 );
	TD4_init( &Target_tracker[1] , 15 , 15 , 15 , 15 );
	TD4_init( &Target_tracker[2] , 15 , 15 , 15 , 15 );		
	
	//初始化期望速度低通滤波器
	Filter_Butter4_LP_float_init( &TargetVelocityFilter[0] , 200 , 10 );
	Filter_Butter4_LP_float_init( &TargetVelocityFilter[1] , 200 , 10 );
	Filter_Butter4_LP_float_init( &TargetVelocityFilter[2] , 200 , 10 );
}