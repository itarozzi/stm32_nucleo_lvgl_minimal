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
#include "ui/screens.h"
#include "ui/ui.h"
#include "ui/vars.h"
#include "lvgl.h"
#include "TouchController.h"
#include "usart.h"
#include <sys/types.h>
// then put this into ui.h header file to include here
extern lv_obj_t *time_label;

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
u_int64_t cnt = 0;

osTimerId_t lvglTimerHandle;
const osTimerAttr_t lvglTimer_attributes = { .name = "lvglTimer" };

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
  lvglTimerHandle = osTimerNew(lvglTimerCallback, osTimerPeriodic, NULL, &lvglTimer_attributes);

  osThreadId_t lvglTaskHandle;
  const osThreadAttr_t lvglTask_attributes = {
    .name       = "lvglTask",
    .stack_size = 2048 * 4,           // 8 KB — increase if you see stack overflow
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
  osTimerStart(lvglTimerHandle, 10);

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
    // printf("STM32 LVGL tick");
    // lv_tick_inc(1);

    

    
    // HAL_UART_Transmit(&huart2, (uint8_t*)"thick\r\n", 8, 1000);

    cnt++;
    if (cnt >= 100) {
      cnt = 0;
      // lv_lock();

      //lv_label_set_text_fmt(objects.label_test, "%d", HAL_GetTick());

      // lv_label_set_text_fmt(time_label, "%d", HAL_GetTick());
      //  lv_unlock();

      HAL_UART_Transmit(&huart2, (uint8_t*)"thick2\r\n", 10, 1000);
    }

}


bool changed_ok = false;
void startLvglTask(void *argument) {
    for (;;) {
        osDelay(5);
        TouchController_Poll();
        
        lv_timer_handler();

        // lv_lock();
        ui_tick();
        // lv_unlock();


        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
        {
            // Button pressed
            if (!changed_ok) {
              changed_ok = true;
              blue_button_press_cnt++;
              set_var_blue_button_press_cnt(blue_button_press_cnt);
            }
        }
        else
        {
            // Button released
            changed_ok = false;
        }



         // HAL_UART_Transmit(&huart2, (uint8_t*)"LVGL task\r\n", 20, 1000);
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

