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

/* ����IIC�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define GPIO_PORT_IIC     GPIOB                       /* GPIO�˿� */
#define RCC_IIC_ENABLE    __HAL_RCC_GPIOB_CLK_ENABLE()       /* GPIO�˿�ʱ�� */
#define IIC_SCL_PIN       GPIO_PIN_6                 /* ���ӵ�SCLʱ���ߵ�GPIO */
#define IIC_SDA_PIN       GPIO_PIN_7                 /* ���ӵ�SDA�����ߵ�GPIO */

/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 1	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
#define IIC_SCL_1()  HAL_GPIO_WritePin(GPIO_PORT_IIC, IIC_SCL_PIN, GPIO_PIN_SET)		/* SCL = 1 */
#define IIC_SCL_0()  HAL_GPIO_WritePin(GPIO_PORT_IIC, IIC_SCL_PIN, GPIO_PIN_RESET)		/* SCL = 0 */

#define IIC_SDA_1()  HAL_GPIO_WritePin(GPIO_PORT_IIC, IIC_SDA_PIN, GPIO_PIN_SET)		/* SDA = 1 */
#define IIC_SDA_0()  HAL_GPIO_WritePin(GPIO_PORT_IIC, IIC_SDA_PIN, GPIO_PIN_RESET)		/* SDA = 0 */

#define IIC_SDA_READ()  HAL_GPIO_ReadPin(GPIO_PORT_IIC, IIC_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
/*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
#define IIC_SCL_1()  GPIO_PORT_IIC->BSRR = IIC_SCL_PIN				/* SCL = 1 */
#define IIC_SCL_0()  GPIO_PORT_IIC->BRR = IIC_SCL_PIN				/* SCL = 0 */

#define IIC_SDA_1()  GPIO_PORT_IIC->BSRR = IIC_SDA_PIN				/* SDA = 1 */
#define IIC_SDA_0()  GPIO_PORT_IIC->BRR = IIC_SDA_PIN				/* SDA = 0 */

#define IIC_SDA_READ()  ((GPIO_PORT_IIC->IDR & IIC_SDA_PIN) != 0)	/* ��SDA����״̬ */
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

    /*��
     	�����ʱ����ͨ��������AX-Pro�߼������ǲ��Եõ��ġ�
    	CPU��Ƶ72MHzʱ�����ڲ�Flash����, MDK���̲��Ż�
    	ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz
    	ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us
     	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us

    IAR���̱���Ч�ʸߣ���������Ϊ7
    */
    for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: IIC_Start
*	����˵��: CPU����IIC���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IIC_Start(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾIIC���������ź� */
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
*	�� �� ��: IIC_Start
*	����˵��: CPU����IIC����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IIC_Stop(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾIIC����ֹͣ�ź� */
    IIC_SDA_0();
    IIC_SCL_1();
    IIC_Delay();
    IIC_SDA_1();
}

/*
*********************************************************************************************************
*	�� �� ��: IIC_SendByte
*	����˵��: CPU��IIC�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IIC_Send_Byte(uint8_t _ucByte)
{
    uint8_t i;

    /* �ȷ����ֽڵĸ�λbit7 */
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
            IIC_SDA_1(); // �ͷ�����
        }
        _ucByte <<= 1;	/* ����һ��bit */
        IIC_Delay();
    }
}

/*
*********************************************************************************************************
*	�� �� ��: IIC_ReadByte
*	����˵��: CPU��IIC�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t IIC_Read_Byte(uint8_t ack)
{
    uint8_t i;
    uint8_t value;

    /* ������1��bitΪ���ݵ�bit7 */
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
*	�� �� ��: IIC_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t IIC_Wait_Ack(void)
{
    uint8_t re;

    IIC_SDA_1();	/* CPU�ͷ�SDA���� */
    IIC_Delay();
    IIC_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    IIC_Delay();
    if (IIC_SDA_READ())	/* CPU��ȡSDA����״̬ */
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
*	�� �� ��: IIC_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IIC_Ack(void)
{
    IIC_SDA_0();	/* CPU����SDA = 0 */
    IIC_Delay();
    IIC_SCL_1();	/* CPU����1��ʱ�� */
    IIC_Delay();
    IIC_SCL_0();
    IIC_Delay();
    IIC_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: IIC_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IIC_NAck(void)
{
    IIC_SDA_1();	/* CPU����SDA = 1 */
    IIC_Delay();
    IIC_SCL_1();	/* CPU����1��ʱ�� */
    IIC_Delay();
    IIC_SCL_0();
    IIC_Delay();
}
void IIC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_IIC_ENABLE;	/* ��GPIOʱ�� */

    GPIO_InitStructure.Pin = IIC_SCL_PIN | IIC_SDA_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;  	/* ��©��� */
    HAL_GPIO_Init(GPIO_PORT_IIC, &GPIO_InitStructure);

    /* ��һ��ֹͣ�ź�, ��λIIC�����ϵ������豸������ģʽ */
    IIC_Stop();
}

/*
*********************************************************************************************************
*	�� �� ��: IIC_CheckDevice
*	����˵��: ���IIC�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��IIC���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
uint8_t IIC_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    IIC_GPIO_Init();		/* ����GPIO */

    IIC_Start();		/* ���������ź� */

    /* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
    IIC_Send_Byte(_Address|IIC_WR);
    ucAck = IIC_Wait_Ack();	/* ����豸��ACKӦ�� */

    IIC_Stop();			/* ����ֹͣ�ź� */

    return ucAck;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

