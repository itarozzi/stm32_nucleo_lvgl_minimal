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
#include "task.h"
#include "main.h"
#include "ui_logic.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ui/ui.h"
#include "ui/vars.h"
#include "lvgl.h"
#include "TouchController.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LVGL_TASK_WAKE_FLAG      (1UL << 0)
#define LVGL_TASK_PERIOD_MS      5U

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osThreadId_t lvglTaskHandle;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void lvglTimerCallback(void *argument);
void startLvglTask(void *argument);

osStatus_t osThreadDetach(osThreadId_t thread_id);

// Stub for missing CMSIS-RTOS2 function not implemented by ST's FreeRTOS port


/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  const osThreadAttr_t lvglTask_attributes = {
    .name       = "lvglTask",
    .stack_size = 2048 * 4,
    .priority   = osPriorityLow,
  };



  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  (void)lvglTimerCallback;

  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  lvglTaskHandle = osThreadNew(startLvglTask, NULL, &lvglTask_attributes);
 
 
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
  (void)argument;
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void lvglTimerCallback(void *argument) {
    (void)argument;
}


static bool button_was_pressed = false;
void startLvglTask(void *argument) {
    uint32_t flags;

    (void)argument;

    for (;;) {
        flags = osThreadFlagsWait(LVGL_TASK_WAKE_FLAG, osFlagsWaitAny, LVGL_TASK_PERIOD_MS);
        (void)flags;

        TouchController_Poll();
        lv_timer_handler();
        ui_tick();

        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
        {
            if (!button_was_pressed) {
              button_was_pressed = true;
              blue_button_press_cnt++;
              set_var_blue_button_press_cnt(blue_button_press_cnt);
            }
        }
        else
        {
            button_was_pressed = false;
        }
    }
}

// osThreadDetach is not implemented in ST's FreeRTOS CMSIS-RTOS2 wrapper.
// LVGL calls it in lv_thread_delete() — provide a no-op stub.
osStatus_t osThreadDetach(osThreadId_t thread_id)
{
    (void)thread_id;
    return osOK;
}

/* USER CODE END Application */

