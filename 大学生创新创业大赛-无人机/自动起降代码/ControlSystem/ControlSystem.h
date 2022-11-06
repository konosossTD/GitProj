#pragma once

#include <stdbool.h>

/*观测器*/
	//获取悬停油门
	float get_hover_throttle();
	//获取是否在飞行
	bool get_is_inFlight();
	//获取ENU x风力（造成的加速度cm/s^2）
	float get_WindDisturbance_x();
	//获取ENU y风力（造成的加速度cm/s^2）
	float get_WindDisturbance_y();
/*观测器*/

/*姿态控制*/
	typedef enum
	{
		Attitude_ControlMode_Angle ,
		Attitude_ControlMode_AngularRate ,
		Attitude_ControlMode_Locking ,
	}Attitude_ControlMode;
	
	bool Attitude_Control_Enable();
	bool Attitude_Control_Disable();

	bool Attitude_Control_set_Throttle( float thr );
	float Attitude_Control_get_Target_Roll();
	float Attitude_Control_get_Target_Pitch();
	bool Attitude_Control_set_Target_RollPitch( float Roll , float Pitch );

	bool Attitude_Control_set_Target_Yaw( float Yaw );
	bool Attitude_Control_set_Target_YawRate( float YawRate );
	bool Attitude_Control_set_YawLock();
/*姿态控制*/
	
/*位置控制*/
	typedef enum
	{
		Position_ControlMode_Position ,
		Position_ControlMode_Velocity ,
		Position_ControlMode_Locking ,
		
		Position_ControlMode_Takeoff ,
	}Position_ControlMode;
	bool Altitude_Control_Enable();
	bool Altitude_Control_Disable();
	
	/*高度*/
		bool Position_Control_set_TargetPositionZ( float posz );
		bool Position_Control_set_TargetVelocityZ( float velz );
		bool Position_Control_set_ZLock();
	
		Position_ControlMode get_Altitude_ControlMode();
	
		//起飞到当前高度上方的height高度
		bool Position_Control_Takeoff_HeightRelative( float height );
	/*高度*/
	
	/*水平位置*/
		bool get_Position_Control_Enabled();
		bool Position_Control_Enable();
		bool Position_Control_Disable();
		
		Position_ControlMode get_Position_ControlMode();
	
		bool Position_Control_set_TargetPositionXY( float posx , float posy );
		bool Position_Control_set_TargetVelocityBodyHeadingXY( float velx , float vely );
		bool Position_Control_set_XYLock();
	/*水平位置*/
/*位置控制*/