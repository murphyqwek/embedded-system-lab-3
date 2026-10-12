#include "FreeRTOS.h"
typedef void *EventGroupHandle_t;
typedef unsigned EventBits_t;
EventBits_t xEventGroupWaitBits(EventGroupHandle_t, EventBits_t, BaseType_t, BaseType_t, TickType_t);
