/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
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

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/*******************************************************************************
��������        ��I2C_DELAY
��������        ����ʱ����
��������ֵ      ��void
����˵��        ����ʱ����                
*******************************************************************************/
void I2C_DELAY(uint16_t del)
{
	uint16_t i;
	
	for(i=0;i<del;i++)
	{
        __NOP();    
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }
}

/*******************************************************************************
��������        ��I2C_START
��������        ��void
��������ֵ      ��void
����˵��        ��I2C��ʼ�ź�        
*******************************************************************************/
void I2C_START(void)
{      
    I2C_SDA(1);
    I2C_SCL(1);  
    I2C_DELAY(2); 
    I2C_SDA(0); 
    I2C_DELAY(2);          
    I2C_SCL(0);        
}

/*******************************************************************************
��������        ��I2C_STOP
��������        ��void
��������ֵ      ��void
����˵��        ��I2Cֹͣ�ź�        
*******************************************************************************/
void I2C_STOP(void)
{      
    I2C_SCL(0); 
    I2C_SDA(0);     
    I2C_DELAY(2); 
    I2C_SCL(1); 
    I2C_DELAY(2);          
    I2C_SDA(1);        
}

/*******************************************************************************
��������        ��I2C_WAIT_ACK
��������        ��void
��������ֵ      ��������ѯ�Ƿ��쳣
����˵��        ��I2C�ȴ��ӻ�����Ӧ���źţ����û���յ�Ӧ���źţ�˵���쳣��
                  ����ֹͣ�ź�ֹͣI2CͨѶ��        
*******************************************************************************/
uint8_t I2C_WAIT_ACK(void)
{   
    uint8_t Wait_cnt;
    
    I2C_SCL(0);      
    I2C_SDA_IN();
    I2C_SDA(0);    
    I2C_DELAY(2);
    I2C_SCL(1);    
    
    Wait_cnt = 255;
    
    while(RD_I2C_SDA != 0)
    {
        Wait_cnt--;
        if(Wait_cnt == 0)
        {
            break;
        }   
    }

    I2C_SCL(0);     
    I2C_SDA_OUT();   
    
    if(Wait_cnt == 0)
    {
        I2C_STOP();
    }    

    return Wait_cnt;
}

/*******************************************************************************
��������        ��I2C_ACK
��������        ��void
��������ֵ      ��void
����˵��        ��������ȡ�ӻ����ݺ󣬲���һ��Ӧ���źŸ��ӻ���������֪ͨ�ӻ�
                   ���Է�����һ�����ݡ�        
*******************************************************************************/
void I2C_ACK(void)
{   
    I2C_SCL(0); 
    I2C_SDA(0);//���͵͵�ƽ����Ӧ�ӻ�
    I2C_DELAY(2);         
    I2C_SCL(1);    
    I2C_DELAY(2);      
    I2C_SCL(0); 
}

/*******************************************************************************
��������        ��I2C_NOT_ACK
��������        ��void
��������ֵ      ��void
����˵��        ��������ȡ�ӻ����ݺ󣬲���һ����Ӧ���źŸ��ӻ���������֪ͨ�ӻ�
                   ����Ҫ������һ�������ˡ�        
*******************************************************************************/
void I2C_NOT_ACK(void)
{   
    I2C_SCL(0); 
    I2C_SDA(1);//���͵͵�ƽ����Ӧ�ӻ�
    I2C_DELAY(2);         
    I2C_SCL(1);    
    I2C_DELAY(2);      
    I2C_SCL(0); 
}

/*******************************************************************************
��������        ��I2C_WRITE_BYTE
��������        �����͵�����
��������ֵ      �������ж��Ƿ��յ���Ӧ��0���յ���Ӧ��1��û���յ���Ӧ
����˵��        ����������һ���ֽ����ݸ��ӻ������ȴ��ӻ���Ӧ        
*******************************************************************************/
uint8_t I2C_WRITE_BYTE(uint8_t dat)
{   
    uint8_t i;
    
    I2C_SCL(0);    
    I2C_SDA(0);//�ͷ�����
    I2C_DELAY(1); 
    
    for(i = 0;i < 8;i ++)
    {  
        if(dat & 0x80)
        {
            I2C_SDA(1);        
        }
        else
        {
            I2C_SDA(0);    
        }
        dat <<= 1;
        I2C_DELAY(4);   
        I2C_SCL(1); 
        I2C_DELAY(8); 
        I2C_SCL(0); 
    }
       
    i = I2C_WAIT_ACK();
    
    return i;
}

/*******************************************************************************
��������        ��I2C_READ_BYTE
��������        ��0��������Ӧ���ź�  ��0������Ӧ���ź�
��������ֵ      �����ض�ȡ��������
����˵��        ��������ȡ�ӻ������ݣ�������һ��Ӧ���źŻ��Ӧ���źŸ��ӻ�        
*******************************************************************************/
uint8_t I2C_READ_BYTE(uint8_t ack)
{   
    uint8_t i;
    uint8_t dat;
    
    I2C_SCL(0); 
    I2C_SDA_IN();
    I2C_SDA(0);       
    I2C_DELAY(1); 
    
    for(i = 0;i < 8;i ++)
    {  
        I2C_SCL(1); 
        I2C_DELAY(4); 
        dat <<= 1;
        if(RD_I2C_SDA != 0)
        {
            dat |= 0x01;         
        }
        I2C_SCL(0); 
        I2C_DELAY(4); 
    }
    
    I2C_SDA_OUT();
    
    if(ack != 0)
    {
        I2C_ACK();    
    }
    else
    {
        I2C_NOT_ACK();    
    }   

    return dat;
}

/* USER CODE END 1 */
