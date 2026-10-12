#include "snake/field.h"
#include "snake/apple_generator.h"
#include "snake/snake.h"
#include "snake/event.h"
#include "snake/key_queue.h"
#include "snake/game.h"
#include "snake/render.h"
#include "snake/util.h"
#include "oled.h"
#include <stdlib.h>

static Snake snakeStorage;
static Snake *snake = &snakeStorage;
static GameState state = GAME_START_MENU;
static Position apple;
static TimerHandle_t ticksTimer;
static TimerHandle_t startGameTimer;

static void drawGameField(void) {
    renderStartField();
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            renderUpdateScreen(x, y, FieldGet((Position){x, y}));
        }
    }
    renderScore(snake->length - 1);
    oled_UpdateScreen();
}

void timerTicksHandler(TimerHandle_t handle) {
    (void)handle;
    gameTickEvent();
}

void timerStartGameHandler(TimerHandle_t handle) {
    (void)handle;
    gameStartEvent();
}

void StartGameTask(void *arguments) {
    (void)arguments;
    state = GAME_START_MENU;
    ticksTimer = xTimerCreate("Game tick", pdMS_TO_TICKS(TICKS_MS),
                             pdTRUE, NULL, timerTicksHandler);
    startGameTimer = xTimerCreate("Game start", pdMS_TO_TICKS(START_GAME_DELAY_MS),
                                 pdFALSE, NULL, timerStartGameHandler);
    if (ticksTimer == NULL || startGameTimer == NULL) error();

    renderStartMenu();
    for (;;) {
        EventBits_t bits = xEventGroupWaitBits(getEvent(), ALL_EVENTS,
                                               pdTRUE, pdFALSE, portMAX_DELAY);
        /* Input is applied before movement when both events arrive together. */
        if (bits & INPUT_EVENT) {
            int key = readKeyFromQueue();
            if (key != EMPTY_QUEUE) {
                if (state == GAME_RUNNING || state == GAME_STARTING) {
                    changeDiretion(key);
                } else {
                    enterAnyKey();
                    continue;
                }
            }
        }
        if ((bits & GAME_START_EVENT) && state == GAME_STARTING) StartGame();
        if ((bits & GAME_TICK_EVENT) && state == GAME_RUNNING) {
            if (GameTick()) {
                if (xTimerStop(ticksTimer, portMAX_DELAY) != pdPASS) error();
                clearEvents();
                clearKeyQueue();
                renderEndGameMenu(state == GAME_WIN ? WIN : LOSE, snake->length - 1);
            } else {
                drawGameField();
            }
        }
    }
}

void enterAnyKey(void) {
    /* Human timing supplies a different seed for each new round. */
    srand((unsigned int)xTaskGetTickCount());
    FieldInit();
    Snake_Init(snake);
    if (!generateNewApplePosition(&apple)) error();
    FieldSet(apple, CELL_APPLE);
    state = GAME_STARTING;
    drawGameField();
    if (xTimerStart(startGameTimer, portMAX_DELAY) != pdPASS) error();
}

void changeDiretion(int key) {
    Snake_SetNewDirection(snake, key);
}

/* true means that the round has ended. */
bool GameTick(void) {
    if (state != GAME_RUNNING) return false;
    Position next = getNewHeadPosition(snake);
    if (comparePositions(next, apple)) {
        if (!Snake_Grow(snake)) {
            state = GAME_OVER;
            return true;
        }
        if (snake->length == 5) {
            state = GAME_WIN;
            return true;
        }
        if (!generateNewApplePosition(&apple)) error();
        FieldSet(apple, CELL_APPLE);
    } else if (!Snake_Move(snake)) {
        state = GAME_OVER;
        return true;
    }
    return false;
}

void StartGame(void) {
    state = GAME_RUNNING;
    if (xTimerStart(ticksTimer, portMAX_DELAY) != pdPASS) error();
}

void error(void) {
    if (startGameTimer != NULL) (void)xTimerStop(startGameTimer, portMAX_DELAY);
    if (ticksTimer != NULL) (void)xTimerStop(ticksTimer, portMAX_DELAY);
    clearEvents();
    clearKeyQueue();
    renderError();
    for (;;) vTaskSuspend(NULL);
}
