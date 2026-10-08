#ifndef TEST_FREERTOS_H
#define TEST_FREERTOS_H
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
typedef int BaseType_t;
typedef unsigned TickType_t;
#define pdTRUE 1
#define pdFALSE 0
#define pdPASS 1
#define portMAX_DELAY ((TickType_t)-1)
#define pdMS_TO_TICKS(x) (x)
#define configASSERT(x) assert(x)
TickType_t xTaskGetTickCount(void);
void vTaskSuspend(void *);
#endif
