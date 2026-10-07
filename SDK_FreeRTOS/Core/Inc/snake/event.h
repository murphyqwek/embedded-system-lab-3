#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#define INPUT_EVENT (1 << 0)
#define GAME_START_EVENT (1 << 1)
#define GAME_TICK_EVENT (1 << 2)

#define ALL_EVENTS (INPUT_EVENT | GAME_START_EVENT | GAME_TICK_EVENT)

void initEvents();

void inputEvent();
void gameStartEvent();
void gameTickEvent();

void clearEvents();

EventGroupHandle_t getEvent();
#endif
