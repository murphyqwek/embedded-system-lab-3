#ifndef INC_SNAKE_GAME_H_
#define INC_SNAKE_GAME_H_

#include "FreeRTOS.h"
#include "timers.h"
#include <stdbool.h>

#define TICKS_MS 300
#define START_GAME_DELAY_MS 2000
#define MIN_GAME_TICKS_MS 30

void StartGameTask(void* arguments);

bool GameTick(void);
void StartGame(void);

void timerTicksHandler(TimerHandle_t handle);
void timerStartGameHandler(TimerHandle_t handle);

void enterAnyKey(void);
void changeDiretion(int key);

void error(void);

#endif
