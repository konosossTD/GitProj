/**
  ****************************(C) COPYRIGHT 2021 Boring_TECH*********************
  * @file       BSP_spi.c/h
  * @brief      ��HAL��SPI�������ж��η�װ������Ӧ���������е���
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
  * @brief          ��װSPI6д����
  * @param[in]     	data ����������
  * @retval         �յ�������
  */
u8 SPI6_WriteData(u8 *data,u16 size)
{
	return HAL_SPI_Transmit(&hspi6,data,size, 1000);
}






