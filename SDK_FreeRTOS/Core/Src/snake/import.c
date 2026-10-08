#include "FreeRTOS.h"
#include "task.h"
#include "snake/import.h"
#include "snake/event.h"
#include "snake/key_queue.h"
#include "kb.h"
#include "i2c.h"

void StartTaskKeyboard(void *argument) {
    (void)argument;
    static const uint8_t rows[] = {ROW1, ROW2, ROW3, ROW4};
    int previous = EMPTY_QUEUE;
    int stable = EMPTY_QUEUE;
    for (;;) {
        int key = EMPTY_QUEUE;
        I2C1_Lock();
        for (int i = 0; i < 4; i++) {
            uint8_t value = Check_Row(rows[i]);
            switch (value) {
                case 0x04: key = i * 3 + 1; break;
                case 0x02: key = i * 3 + 2; break;
                case 0x01: key = i * 3 + 3; break;
                default: break;
            }
            if (key != EMPTY_QUEUE) break;
        }
        I2C1_Unlock();
        /* Accept a transition only after two matching scans. */
        if (key == previous && key != stable) {
            stable = key;
            if (key != EMPTY_QUEUE) sendToQueue(key);
        }
        previous = key;
        vTaskDelay(pdMS_TO_TICKS(25));
    }
}

void sendToQueue(int key) {
    appendKeyToQueue(key);
    inputEvent();
}
