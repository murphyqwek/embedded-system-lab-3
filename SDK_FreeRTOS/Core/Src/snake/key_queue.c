#include "FreeRTOS.h"
#include <stdint.h>
#include "queue.h"
#include "snake/key_queue.h"

QueueHandle_t keyQueue;

void initKeyQueue() {
    keyQueue = xQueueCreate(1, sizeof(uint8_t));
}

void appendKeyToQueue(int key) {
    xQueueSend(keyQueue, &key, (TickType_t) 0);
}

int readKeyFromQueue() {
	int key;
    if(xQueueReceive(keyQueue, &key, (TickType_t) 0) == pdPASS) {
    	return key;
    }
    return EMPTY_QUEUE;
}
