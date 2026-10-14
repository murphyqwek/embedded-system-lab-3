/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include "FreeRTOS.h"
#include "task.h"
#include "snake/game.h"
#include "snake/import.h"
#include "snake/event.h"
#include "snake/key_queue.h"
#include "i2c.h"
#include "music.h"

void configureTimerForRunTimeStats(void) {}
unsigned long getRunTimeCounterValue(void) { return 0; }

void vApplicationGetIdleTaskMemory(StaticTask_t **tcb, StackType_t **stack,
                                  uint32_t *size) {
    static StaticTask_t idleTcb;
    static StackType_t idleStack[configMINIMAL_STACK_SIZE];
    *tcb = &idleTcb;
    *stack = idleStack;
    *size = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **tcb, StackType_t **stack,
                                   uint32_t *size) {
    static StaticTask_t timerTcb;
    static StackType_t timerStack[configTIMER_TASK_STACK_DEPTH];
    *tcb = &timerTcb;
    *stack = timerStack;
    *size = configTIMER_TASK_STACK_DEPTH;
}

void MX_FREERTOS_Init(void) {
    initEvents();
    initKeyQueue();
    I2C1_InitMutex();
    BaseType_t result = xTaskCreate(StartGameTask, "Snake", 768, NULL, 2, NULL);
    configASSERT(result == pdPASS);
    result = xTaskCreate(StartTaskKeyboard, "Keyboard", 256, NULL, 2, NULL);
    configASSERT(result == pdPASS);
    result = xTaskCreate(StartMusicTask, "Music", 256, NULL, 1, NULL);
    configASSERT(result == pdPASS);
}
