/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include <stdio.h>
#include "wiegand.h"
#include <stdarg.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#ifndef __cplusplus
typedef unsigned char bool;
static const bool false = 0;
static const bool true = 1;
#endif
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

bool verifyCode(uint32_t code);
void UART_Printf(const char *fmt, ...);
bool verifyMode();
void verifyKey();
void switchedModes();
void switchMode();
void blink(int blinkTimes);
void read();
void openDoor();

volatile uint8_t wig_flag_inrt = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

  /*
    0 - normal
    1 - closed
    2 - open
    3 - conditionally open
    4 - temporarily open
    5 - alarm
  */
  int workMode = 0;

  // is door opened?
  bool isDoorOpened = false;

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  
  uint32_t timme = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    if( (HAL_GetTick() - timme) > 15000 ){
      switchMode();
      timme = HAL_GetTick();
    }

    if(wig_available())
    {
      if(workMode == 0)
      {
        wig_flag_inrt = 0;
        uint32_t wcode = getCode();
        wig_flag_inrt = 1;

        if(verifyCode(wcode)){
          char s[] = "Door was opened by a key. Key ID: ";
          char code[16];
          sprintf(code,"%lu", wcode);

          strcat(s, code);
          strcat(s, ".\r\n");
          UART_Printf(s);

          openDoor();
        }
        else
        {
          UART_Printf("Attempted to open the door with a wrong key.");
        }
      }
      else if(workMode == 1)
      {
        UART_Printf("Door is closed.\r\n");
        blink(2);
      }
      else if(workMode == 2)
      {
        UART_Printf("Door is opened by default.\r\n");
        HAL_GPIO_WritePin(GPIOB, GreenLed_1_Pin, RESET);
        HAL_GPIO_WritePin(GPIOB, GreenLed_2_Pin, RESET);
      }
      else if(workMode == 3)
      {
        UART_Printf("Door is now opened by default.\r\n");
        blink(1);
        HAL_GPIO_WritePin(GPIOB, GreenLed_1_Pin, RESET);
        HAL_GPIO_WritePin(GPIOB, GreenLed_2_Pin, RESET);
        workMode = 2;
      }
    }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Zumer1_Pin|Zumer2_Pin|GreenLed_1_Pin|GreenLed_2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : D_01_Pin D_11_Pin D_02_Pin D_12_Pin
                           Door_Pin */
  GPIO_InitStruct.Pin = D_01_Pin|D_11_Pin|D_02_Pin|D_12_Pin
                          |Door_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : BT_0_Pin BT_1_Pin */
  GPIO_InitStruct.Pin = BT_0_Pin|BT_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : Zumer1_Pin Zumer2_Pin GreenLed_1_Pin GreenLed_2_Pin */
  GPIO_InitStruct.Pin = Zumer1_Pin|Zumer2_Pin|GreenLed_1_Pin|GreenLed_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == BT_0_Pin || GPIO_Pin == BT_1_Pin)
  {
    UART_Printf("Door was opened by a button.\r\n");

    isDoorOpened = true;

    openDoor();
  }
  else if(GPIO_Pin == Door_Pin)
  {
    isDoorOpened = false;

    if(workMode != 2)
    {
      HAL_GPIO_WritePin(GPIOB, GreenLed_1_Pin, SET);
      HAL_GPIO_WritePin(GPIOB, GreenLed_2_Pin, SET);
    }

    isDoorOpened = false;
  }
  else if( wig_flag_inrt && ( GPIO_Pin == D_01_Pin || GPIO_Pin == D_02_Pin) )
  {
    ReadD0();
  }
  else if( wig_flag_inrt && ( GPIO_Pin == D_11_Pin || GPIO_Pin == D_12_Pin) )
  {
    ReadD1();
  }
  else
  {
    __NOP();
  }
}

/*
  verify if code is valid
*/
bool verifyCode(uint32_t code){
  uint32_t codes[] = {
    12563593
  };

  for(int i = 0; i < sizeof(codes)/sizeof(codes[0]); i++){
    if(code == codes[i])
      return true;
  }

  return false;
}

/*
  print a string to the terminal
*/
void UART_Printf(const char *fmt, ...){
  char buff[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buff, sizeof(buff), fmt, args);
  HAL_UART_Transmit(&huart1, (uint8_t *)buff, strlen(buff), HAL_MAX_DELAY);
  va_end(args);
}

/*
  check for the mode and work by it
*/
bool verifyMode()
{
  switch(workMode){
    case 0:
    {
      return true;
      break;
    }
    case 1:
    { 
      UART_Printf("Door is closed.\r\n");
      blink(3);
      return false;
      break;
    }
    case 2:
    {
      UART_Printf("Door is opened by default.\r\n");
      return false;
      break;
    }
    case 3:
    {
      blink(1);
      workMode = 2;
      return false;
      break;
    }
    default:
    {
      return false;
      break;
    }
  }

  return false;
}

/*
  increment mode by one if it's not 3, then set it to 0
*/
void switchMode()
{
  if(workMode < 3){
    workMode++;
    switchedModes();
    return;
  }
  else if(workMode >= 3){
    workMode = 0;
    switchedModes();
    return;
  }
  else{
    UART_Printf("Unable to switch modes. \r\n");
  }
}

/*
  print switching modes
*/
void switchedModes()
{
  UART_Printf("Switched to mode %d\r\n", workMode);
  blink(1);
  HAL_Delay(500);

  blink(workMode);

  if(workMode == 2)
  {
    HAL_GPIO_WritePin(GPIOB, GreenLed_1_Pin, RESET);
    HAL_GPIO_WritePin(GPIOB, GreenLed_2_Pin, RESET);
  }
}

/*
  blink blinkTimes times
*/
void blink(int blinkTimes)
{
  for(int i = 0; i < blinkTimes; i++)
  {
    HAL_GPIO_WritePin(GPIOB, GreenLed_1_Pin, RESET);
    HAL_GPIO_WritePin(GPIOB, GreenLed_2_Pin, RESET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(GPIOB, GreenLed_1_Pin, SET);
    HAL_GPIO_WritePin(GPIOB, GreenLed_2_Pin, SET);
    HAL_Delay(500);
  }
}

void read()
{
  // reading = true;
  // HAL_Delay(36);

  UART_Printf("test\r\n");

  if(wig_available())
  {
    wig_flag_inrt = 0;
    uint32_t wcode = getCode();
    wig_flag_inrt = 1;

    HAL_GPIO_WritePin(GPIOB, GreenLed_1_Pin, RESET);
    HAL_GPIO_WritePin(GPIOB, GreenLed_2_Pin, RESET);

    if(verifyCode(wcode)){
      char s[] = "Door was opened by key. Key ID: ";
      char code[16];
      sprintf(code,"%lu", wcode);

      strcat(s, code);
      strcat(s, ".\r\n");
      UART_Printf(s);
    }
    else
    {
      UART_Printf("Attempted to open the door with a wrong key.");
    }
  }

  //reading = false;
}

void openDoor()
{
  isDoorOpened = true;

  uint32_t auxTime = HAL_GetTick();

  HAL_GPIO_WritePin(GPIOB, GreenLed_1_Pin, RESET);
  HAL_GPIO_WritePin(GPIOB, GreenLed_2_Pin, RESET);

  while((HAL_GetTick() - auxTime) < 3000)
  {
    if(!isDoorOpened)
    {
      HAL_GPIO_WritePin(GPIOB, GreenLed_1_Pin, SET);
      HAL_GPIO_WritePin(GPIOB, GreenLed_2_Pin, SET);
      UART_Printf("Door was closed.\r\n");
      break;
    }
  }

  // uint32_t auxTime = HAL_GetTick();

  // HAL_GPIO_WritePin(GPIOB, GreenLed_1_Pin, RESET);
  // HAL_GPIO_WritePin(GPIOB, GreenLed_2_Pin, RESET);

  // UART_Printf("Door is opened.\r\n");

  // while((HAL_GetTick() - auxTime) < 3000)
  // {
  //   if(!isDoorOpened)
  //   {
      // HAL_GPIO_WritePin(GPIOB, GreenLed_1_Pin, SET);
      // HAL_GPIO_WritePin(GPIOB, GreenLed_2_Pin, SET);
  //   }
  // }

  // if(isDoorOpened)
  // {
  //   UART_Printf("Warning! Door is still opened!\r\n");
  // }
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
