/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* 定义IIC总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define GPIO_PORT_IIC     GPIOB                       /* GPIO端口 */
#define RCC_IIC_ENABLE    __HAL_RCC_GPIOB_CLK_ENABLE()       /* GPIO端口时钟 */
#define IIC_SCL_PIN       GPIO_PIN_6                 /* 连接到SCL时钟线的GPIO */
#define IIC_SDA_PIN       GPIO_PIN_7                 /* 连接到SDA数据线的GPIO */

/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 1	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
#define IIC_SCL_1()  HAL_GPIO_WritePin(GPIO_PORT_IIC, IIC_SCL_PIN, GPIO_PIN_SET)		/* SCL = 1 */
#define IIC_SCL_0()  HAL_GPIO_WritePin(GPIO_PORT_IIC, IIC_SCL_PIN, GPIO_PIN_RESET)		/* SCL = 0 */

#define IIC_SDA_1()  HAL_GPIO_WritePin(GPIO_PORT_IIC, IIC_SDA_PIN, GPIO_PIN_SET)		/* SDA = 1 */
#define IIC_SDA_0()  HAL_GPIO_WritePin(GPIO_PORT_IIC, IIC_SDA_PIN, GPIO_PIN_RESET)		/* SDA = 0 */

#define IIC_SDA_READ()  HAL_GPIO_ReadPin(GPIO_PORT_IIC, IIC_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
/*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
#define IIC_SCL_1()  GPIO_PORT_IIC->BSRR = IIC_SCL_PIN				/* SCL = 1 */
#define IIC_SCL_0()  GPIO_PORT_IIC->BRR = IIC_SCL_PIN				/* SCL = 0 */

#define IIC_SDA_1()  GPIO_PORT_IIC->BSRR = IIC_SDA_PIN				/* SDA = 1 */
#define IIC_SDA_0()  GPIO_PORT_IIC->BRR = IIC_SDA_PIN				/* SDA = 0 */

#define IIC_SDA_READ()  ((GPIO_PORT_IIC->IDR & IIC_SDA_PIN) != 0)	/* 读SDA口线状态 */
#endif

I2C_HandleTypeDef hi2c1;

/* USER CODE END 0 */
/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x307075B1;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* I2C1 interrupt Init */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

    /* I2C1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void IIC_GPIO_Init(void);
static void IIC_Delay(void)
{
    uint8_t i;

    /*　
     	下面的时间是通过安富莱AX-Pro逻辑分析仪测试得到的。
    	CPU主频72MHz时，在内部Flash运行, MDK工程不优化
    	循环次数为10时，SCL频率 = 205KHz
    	循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us
     	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us

    IAR工程编译效率高，不能设置为7
    */
    for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	函 数 名: IIC_Start
*	功能说明: CPU发起IIC总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void IIC_Start(void)
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示IIC总线启动信号 */
    IIC_SDA_1();
    IIC_SCL_1();
    IIC_Delay();
    IIC_SDA_0();
    IIC_Delay();
    IIC_SCL_0();
    IIC_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: IIC_Start
*	功能说明: CPU发起IIC总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void IIC_Stop(void)
{
    /* 当SCL高电平时，SDA出现一个上跳沿表示IIC总线停止信号 */
    IIC_SDA_0();
    IIC_SCL_1();
    IIC_Delay();
    IIC_SDA_1();
}

/*
*********************************************************************************************************
*	函 数 名: IIC_SendByte
*	功能说明: CPU向IIC总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void IIC_Send_Byte(uint8_t _ucByte)
{
    uint8_t i;

    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++)
    {
        if (_ucByte & 0x80)
        {
            IIC_SDA_1();
        }
        else
        {
            IIC_SDA_0();
        }
        IIC_Delay();
        IIC_SCL_1();
        IIC_Delay();
        IIC_SCL_0();
        if (i == 7)
        {
            IIC_SDA_1(); // 释放总线
        }
        _ucByte <<= 1;	/* 左移一个bit */
        IIC_Delay();
    }
}

/*
*********************************************************************************************************
*	函 数 名: IIC_ReadByte
*	功能说明: CPU从IIC总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t IIC_Read_Byte(uint8_t ack)
{
    uint8_t i;
    uint8_t value;

    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        IIC_SCL_1();
        IIC_Delay();
        if (IIC_SDA_READ())
        {
            value++;
        }
        IIC_SCL_0();
        IIC_Delay();
    }
    if(ack==0)
        IIC_NAck();
    else
        IIC_Ack();
    return value;
}

/*
*********************************************************************************************************
*	函 数 名: IIC_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t IIC_Wait_Ack(void)
{
    uint8_t re;

    IIC_SDA_1();	/* CPU释放SDA总线 */
    IIC_Delay();
    IIC_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    IIC_Delay();
    if (IIC_SDA_READ())	/* CPU读取SDA口线状态 */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    IIC_SCL_0();
    IIC_Delay();
    return re;
}

/*
*********************************************************************************************************
*	函 数 名: IIC_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void IIC_Ack(void)
{
    IIC_SDA_0();	/* CPU驱动SDA = 0 */
    IIC_Delay();
    IIC_SCL_1();	/* CPU产生1个时钟 */
    IIC_Delay();
    IIC_SCL_0();
    IIC_Delay();
    IIC_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: IIC_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void IIC_NAck(void)
{
    IIC_SDA_1();	/* CPU驱动SDA = 1 */
    IIC_Delay();
    IIC_SCL_1();	/* CPU产生1个时钟 */
    IIC_Delay();
    IIC_SCL_0();
    IIC_Delay();
}
void IIC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_IIC_ENABLE;	/* 打开GPIO时钟 */

    GPIO_InitStructure.Pin = IIC_SCL_PIN | IIC_SDA_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;  	/* 开漏输出 */
    HAL_GPIO_Init(GPIO_PORT_IIC, &GPIO_InitStructure);

    /* 给一个停止信号, 复位IIC总线上的所有设备到待机模式 */
    IIC_Stop();
}

/*
*********************************************************************************************************
*	函 数 名: IIC_CheckDevice
*	功能说明: 检测IIC总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的IIC总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t IIC_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    IIC_GPIO_Init();		/* 配置GPIO */

    IIC_Start();		/* 发送启动信号 */

    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
    IIC_Send_Byte(_Address|IIC_WR);
    ucAck = IIC_Wait_Ack();	/* 检测设备的ACK应答 */

    IIC_Stop();			/* 发送停止信号 */

    return ucAck;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

