/**
  ****************************(C) COPYRIGHT 2021 Boring_TECH*********************
  * @file       BSP_TIM.c/h
  * @brief      TIM的二次封装
  * @note      	
  * @history
  *  Version    Date            Author          Modification
  *  V3.0.0     2020.7.14     	              	1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2021 Boring_TECH*********************
  */

#include "BSP_TIM.h"
#include "tftlcd.h"

#define channel_count 8

u16 channel_buf[9];
static u32 val=0,last_val=0;
static u8 channel=0;
u8 RcState=0;





void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==htim2.Instance)
	{

	}
	if(htim->Instance==htim4.Instance)
	{

	}

}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==htim2.Instance)
	{
		val=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);

		if( val-last_val>5000 && val>last_val && channel==254)
		{
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
			last_val=val;
			channel=0;

		}
		else if(channel>=channel_count)
		{
			last_val=val;
			
			channel=254;

			
			
//			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
		}
		else if(channel<channel_count)
		{
			
			channel_buf[channel]=(val-last_val-1000)/10;
			if(channel_buf[channel]>10000)channel_buf[channel]=0;
			channel++;
			last_val=val;
			
		}

	}

}
