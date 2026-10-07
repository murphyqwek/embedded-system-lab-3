#include "snake/import.h"
#include "snake/event.h"
#include "snake/key_queue.h"
#include "task.h"
#include "kb.h"
#include "portmacro.h"

void StartTaskKeyboard(void * argument) {
    uint8_t key_number, temp_value, Row[4] = {ROW1, ROW2, ROW3, ROW4};
    
    const TickType_t keyboardDelay = 25 / portTICK_PERIOD_MS;
    
    for(;;) {
        for (int i = 0; i < 4; i++) {
            temp_value = Check_Row(Row[i]);
            switch (temp_value) {
                case 0x04: appendKeyToQueue(i*3+1);
                case 0x02: appendKeyToQueue(i*3+2);
                case 0x01: appendKeyToQueue(i*3+3);
                default: break;
            }
        }

        xTaskDelay(keyboardDelay);
    }
}