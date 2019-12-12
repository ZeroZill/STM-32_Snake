/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "snake.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId GameTaskHandle;
osThreadId TimeTaskHandle;
osThreadId LEDTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void GameFunc(void const * argument);
void TimeFunc(void const * argument);
void LEDFunc(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of GameTask */
  osThreadDef(GameTask, GameFunc, osPriorityNormal, 0, 128);
  GameTaskHandle = osThreadCreate(osThread(GameTask), NULL);

  /* definition and creation of TimeTask */
  osThreadDef(TimeTask, TimeFunc, osPriorityNormal, 0, 128);
  TimeTaskHandle = osThreadCreate(osThread(TimeTask), NULL);

  /* definition and creation of LEDTask */
  osThreadDef(LEDTask, LEDFunc, osPriorityNormal, 0, 128);
  LEDTaskHandle = osThreadCreate(osThread(LEDTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_GameFunc */
/**
  * @brief  Function implementing the GameTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_GameFunc */
void GameFunc(void const * argument)
{
    
    
    
    

  /* USER CODE BEGIN GameFunc */
  /* Infinite loop */
  for(;;)
  {
	  launch();
  }
  /* USER CODE END GameFunc */
}

/* USER CODE BEGIN Header_TimeFunc */
/**
* @brief Function implementing the TimeTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TimeFunc */
void TimeFunc(void const * argument)
{
  /* USER CODE BEGIN TimeFunc */
  /* Infinite loop */
  for(;;)
  {
	milisecond++;
    osDelay(1);
  }
  /* USER CODE END TimeFunc */
}

/* USER CODE BEGIN Header_LEDFunc */
/**
* @brief Function implementing the LEDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LEDFunc */
void LEDFunc(void const * argument)
{
  /* USER CODE BEGIN LEDFunc */
  /* Infinite loop */
  for(;;)
  {
	if(green_blink){
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		osDelay(250);
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		osDelay(250);
		green_blink = 0;
	}
	if(red_blink){
		HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
		osDelay(250);
		HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
		osDelay(250);
		red_blink = 0;
	}
    osDelay(1);
  }
  /* USER CODE END LEDFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
