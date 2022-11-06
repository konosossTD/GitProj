
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "BSP_TIM.h"
#include "BSP_GPIO.h"
#include "BSP_USART.h"
#include "BSP_spi.h"
#include "stdio.h"	
#include "tftlcd.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"


void SystemClock_Config(void);

int main(void)
{
	float pitch,roll,yaw; 
	char str_roll[10],str_yaw[10],str_pitch[10];
	short aacx,aacy,aacz;			//加速度传感器原始数据
	short gyrox,gyroy,gyroz;		//陀螺仪原始数据
	short temp;					//温度

	SCB_EnableICache();
	SCB_EnableDCache();


	HAL_Init();
	SystemClock_Config();

	

	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_SPI6_Init();
	MX_I2C1_Init();
	MX_TIM2_Init();
	LCD_Init();

	
	HAL_I2C_MspInit(&hi2c1);
		//PPM定时器初始化
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);

	MPU_Init();
	int s=mpu_dmp_init();
	while(s)
	{
		HAL_Delay(20);
		break;
	}

	LCD_Clear(BLACK); 		
	BACK_COLOR=BLACK;
	POINT_COLOR=GREEN;	
	LCD_ShowString(0, 0, 60, 12, 12, "konosossTD");
	LCD_ShowString(200, 0, 60, 12, 12, "RC:");
	LCD_ShowString(0,35,240,12,12,"Roll Pitch PWR  Yaw  Sw1  Sw2  Vr1  Vr2 ");


	while(1)
	{
		__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,10);
		__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,20);
		POINT_COLOR=WHITE;	

		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
//			printf("p:%f,r:%f,y:%f\r\n",pitch,roll,yaw);
			LCD_ShowString(10, 90, 50, 12, 12, "YAW: ");
			sprintf(str_yaw, "%.3f", yaw);
			LCD_ShowString(100,90,50,12,12,str_yaw);
			
			LCD_ShowString(10, 105, 50, 12, 12, "PITCH: ");
			sprintf(str_pitch, "%.3f", pitch);
			LCD_ShowString(100,105,50,12,12,str_pitch);
			
			LCD_ShowString(10, 120, 50, 12, 12, "ROLL: ");
			sprintf(str_roll, "%.3f", roll);
			LCD_ShowString(100,120,50,12,12,str_roll);

		}
		for(int i=0;i<8;i++)
		{

			LCD_ShowNum(30*i,50,channel_buf[i],3,12);
		
		}

		
		HAL_Delay(10);
		if(HAL_GPIO_ReadPin(KEY_GPIO_Port,KEY_Pin)==1){
//			printf("R1 B1 G1 \r\n");
			POINT_COLOR=GREEN;	
			LCD_ShowString(0, 65, 160, 24, 24, "RELEASED");
			LED_R(1);
			LED_B(1);
			LED_G(1);
		}
		else
		{

			POINT_COLOR=YELLOW;	
			LCD_ShowString(0, 65, 160, 24, 24, "PRESSING ");
			LED_R(0);
			LED_B(0);
			LED_G(0);
		}  
//		printf("|%d|",channel_buf[0]);
//		printf("%d|",channel_buf[1]);
//		printf("%d|",channel_buf[2]);
//		printf("%d|",channel_buf[3]);
//		printf("%d|",channel_buf[4]);
//		printf("%d|",channel_buf[5]);
//		printf("%d|",channel_buf[6]);
//		printf("%d|",channel_buf[7]);
//		printf("%d|\r\n",channel_buf[8]);
//		HAL_Delay(10);
	}
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
