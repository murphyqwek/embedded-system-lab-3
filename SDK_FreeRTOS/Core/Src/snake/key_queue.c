#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "snake/key_queue.h"

static QueueHandle_t keyQueue;

void initKeyQueue(void) {
    keyQueue = xQueueCreate(1, sizeof(int));
    configASSERT(keyQueue != NULL);
}

void appendKeyToQueue(int key) {
    /* A one-element mailbox retains the latest button, not stale input. */
    BaseType_t result = xQueueOverwrite(keyQueue, &key);
    configASSERT(result == pdPASS);
}

int readKeyFromQueue(void) {
    int key = EMPTY_QUEUE;
    (void)xQueueReceive(keyQueue, &key, 0);
    return key;
}

void clearKeyQueue(void) {
    xQueueReset(keyQueue);
}
