/**
  ****************************(C) COPYRIGHT 2021 Boring_TECH*********************
  * @file       BSP_USART.c/h
  * @brief      将HAL库串口函数进行二次封装，并在串口中断中接收数据
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
#include "BSP_USART.h"


struct __FILE 
{ 
	int handle; 
}; 
  FILE __stdout;     
int fputc(int ch, FILE *f)
{ 	
	while((USART1->ISR&0X40)==0);//循环发送,直到发送完毕   
	USART1->TDR=(u8)ch;      
	return ch;
}


u8 rx1_buf;

/**
	* @brief          串口接收中断回调函数
  * @param[in]     	huart 串口序号
  * @retval         none
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(huart->Instance==USART1)
	{
		HAL_UART_Receive_IT(&huart1, &rx1_buf, 1);	
		HAL_UART_Transmit(&huart1, &rx1_buf, 1, 1000);

	}
	

}

/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/

