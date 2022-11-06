/**
  ****************************(C) COPYRIGHT 2021 Boring_TECH*********************
  * @file       BSP_spi.c/h
  * @brief      将HAL库SPI函数进行二次封装，给相应的器件进行调用
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
#include "BSP_spi.h"
#include "spi.h"

/**
  * @brief          封装SPI6写函数
  * @param[in]     	data 发出的数据
  * @retval         收到的数据
  */
u8 SPI6_WriteData(u8 *data,u16 size)
{
	return HAL_SPI_Transmit(&hspi6,data,size, 1000);
}






