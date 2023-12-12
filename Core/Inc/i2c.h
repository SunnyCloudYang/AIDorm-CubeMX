/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_I2C1_Init(void);
void I2C_DELAY(uint16_t del);
void I2C_START(void);
void I2C_STOP(void);
uint8_t I2C_WAIT_ACK(void);
void I2C_ACK(void);
void I2C_NOT_ACK(void);
uint8_t I2C_WRITE_BYTE(uint8_t dat);
uint8_t I2C_READ_BYTE(uint8_t ack);

/* USER CODE BEGIN Prototypes */

#define I2C_SCL(x)      (x ?  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET) )       
#define I2C_SDA(x)      (x ?  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET) )       
#define RD_I2C_SDA      HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)   

#define I2C_SDA_OUT()   {GPIOB->CRL &= 0x0FFFFFFF;GPIOB->CRL |= 0x30000000;} 
#define I2C_SDA_IN()    {GPIOB->CRL &= 0x0FFFFFFF;GPIOB->CRL |= 0x40000000;}

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

