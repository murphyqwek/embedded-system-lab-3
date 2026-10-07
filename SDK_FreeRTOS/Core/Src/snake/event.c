#include "FreeRTOS.h"
#include "event_groups.h"

#include "snake/event.h"

EventGroupHandle_t event;

void initEvents() {
    event = xEventGroupCreate();
}

void inputEvent() {
    xEventGroupSetBits(event, INPUT_EVENT);
}

void gameStartEvent() {
    xEventGroupSetBits(event, GAME_START_EVENT);
}

void gameTickEvent() {
    xEventGroupSetBits(event, GAME_TICK_EVENT);
}

void clearEvents() {
    xEventGroupClearBits(event, INPUT_EVENT | GAME_START_EVENT | GAME_TICK_EVENT);
}
