/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "SHTC3.h"
#include "SGP30.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void showTempAndHumid(uint8_t lineNum);
void showLightStrength(uint8_t lineNum);
void showAirQuality(uint8_t lineNum);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  sgp30_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t cnt = 0;
  uint8_t interval = 10;
  while (1)
  {
	OLED_Clear();
    if (cnt<interval)
	{
		OLED_ShowString(20, 0, "IntelliDorm", OLED_8X16);
		showTempAndHumid(1);
		showLightStrength(3);
	}
	else 
	{
		OLED_ShowString(8, 0, "SunnyCloudYang", OLED_8X16);
		showLightStrength(1);
		showAirQuality(2);
	}

    OLED_Update();
	cnt++;
	if (cnt>2*interval) cnt=0;
    HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void showTempAndHumid(uint8_t lineNum)
{
	int32_t temp, humid;
	if (!shtc3_read_id(&hi2c1)) {
      OLED_ShowString(1, lineNum*16+8, "SHTC3 not found", OLED_8X16);
    }
    else
    {
      OLED_ShowString(1, lineNum*16, "Temp: ", OLED_8X16);
      OLED_ShowString(1, (lineNum+1)*16, "Humid: ", OLED_8X16);
      if (shtc3_perform_measurements(&hi2c1, &temp, &humid))
      {
        OLED_ShowFloatNum(70, lineNum*16, temp/100.0, 2, 1, OLED_8X16);
        if (temp>0)
          OLED_ShowChar(70, lineNum*16, ' ', OLED_8X16);
        OLED_DrawCircle(115, lineNum*16+3, 2, 0);
        OLED_ShowChar(120, lineNum*16, 'C', OLED_8X16);
        OLED_ShowNum(102, (lineNum+1)*16, humid, 2, OLED_8X16);
        OLED_ShowChar(120, (lineNum+1)*16, '%', OLED_8X16);
      }
      else
      {
        OLED_ShowString(50, lineNum*16, "ERR", OLED_8X16);
        OLED_ShowString(50, (lineNum+1)*16, "ERR", OLED_8X16);
      }
    }
}

void showLightStrength(uint8_t lineNum)
{
	uint32_t light;
	uint8_t light_digits = 3;
	
	OLED_ShowString(1, lineNum*16, "Light: ", OLED_8X16);
	
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 100);
    light = 4095 - HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
	
	if (light>999) light_digits = 4;
	
    OLED_ShowNum(112-light_digits*8-2, lineNum*16, light, light_digits, OLED_8X16);
    OLED_ShowString(112, lineNum*16, "Lx", OLED_8X16);
}

void showAirQuality(uint8_t lineNum)
{
	uint16_t CO2, TVOC;
	
	OLED_ShowString(1, lineNum*16, "CO :", OLED_8X16);
	OLED_ShowChar(17, lineNum*16+8, '2', OLED_6X8);
	OLED_ShowString(1, (lineNum+1)*16, "TVOC:", OLED_8X16);
	
	sgp30_read(&CO2, &TVOC);
	
	uint8_t CO2_DIGITS = 3, TVOC_DIGITS = 1;
	
	if (TVOC>9) TVOC_DIGITS = 2;
	if (TVOC>99) TVOC_DIGITS = 3;
	if (TVOC>999) TVOC_DIGITS = 4;
	if (TVOC>9999) TVOC_DIGITS = 5;
	if (CO2>999) CO2_DIGITS = 4;
	if (CO2>9999) CO2_DIGITS = 5;
	
	OLED_ShowNum(104-CO2_DIGITS*8-2, lineNum*16, CO2, CO2_DIGITS, OLED_8X16);
	OLED_ShowNum(104-TVOC_DIGITS*8-2, (lineNum+1)*16, TVOC, TVOC_DIGITS, OLED_8X16);
	
	OLED_ShowString(104, lineNum*16, "ppm", OLED_8X16);
	OLED_ShowString(104, (lineNum+1)*16, "ppb", OLED_8X16);
}

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
