/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "tim.h"

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

extern uint8_t send_flag;         // 声明外部变量
extern float g_kp_value;          // 声明外部变量
extern UART_HandleTypeDef huart1; // 声明外部变量
void Send_Float_Via_JustFloat(UART_HandleTypeDef *huart, float data);

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for LEDTask */
osThreadId_t LEDTaskHandle;
const osThreadAttr_t LEDTask_attributes = {
    .name = "LEDTask",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t)osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of LEDTask */
  LEDTaskHandle = osThreadNew(StartTask02, NULL, &LEDTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  uint16_t brightness = 0;
  for (;;)
  {
    for (brightness = 0; brightness <= 1000; brightness += 10)
    {

      // ========== 蓝灯呼吸 ==========
      // 渐亮（0 -> 1000）
      for (brightness = 0; brightness <= 1000; brightness += 10)
      {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, brightness);
        osDelay(10);
      }
      // 渐暗（1000 -> 0）
      for (brightness = 1000; brightness > 0; brightness -= 10)
      {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, brightness);
        osDelay(10);
      }

      // ========== 绿灯呼吸 ==========
      // 渐亮
      for (brightness = 0; brightness <= 1000; brightness += 10)
      {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, brightness);
        osDelay(10);
      }
      // 渐暗
      for (brightness = 1000; brightness > 0; brightness -= 10)
      {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, brightness);
        osDelay(10);
      }

      //========== 红灯呼吸 ==========
      // 渐亮
      for (brightness = 0; brightness <= 1000; brightness += 10)
      {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, brightness);
        osDelay(10);
      }
      // 渐暗
      for (brightness = 1000; brightness > 0; brightness -= 10)
      {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, brightness);
        osDelay(10);
      }

      osDelay(200); // 三个颜色都结束后，稍作停顿再循环

      //==========串口==========
      if (send_flag == 1) // 如果串口中断接收到了数据
      {
        send_flag = 0;                                 // 旗子放倒
        Send_Float_Via_JustFloat(&huart1, g_kp_value); // 发送数据
      }

      osDelay(10); // 让出CPU，不然系统会卡死
    }
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
 * @brief Function implementing the LEDTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
