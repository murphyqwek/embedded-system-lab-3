#include "snake/key_queue.h"
#include "queue.h"

QueueHandle_t keyQueue;

void initKeyQueue() {
    keyQueue = xQueueCreate(1, sizeof(uint8_t));
}

void appendKeyToQueue(uint8_t* key) {
    xQueueSend(keyQueue, key, (TickType_t) 0);
}

void readKeyFromQueue(unit8_t* key) {
    xQueueReceive(keyQueue, key, (TickType_t) 0);
}