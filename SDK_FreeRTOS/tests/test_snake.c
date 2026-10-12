/* Host regression tests: actual game/render code, simulated RTOS and OLED. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../Core/Src/snake/game.c"
#include "snake/snake_alloc.h"

static unsigned frames;
static unsigned char pixels[OLED_HEIGHT][OLED_WIDTH];
static char screenText[256];
FontDef Font_7x10 = {7, 10, NULL};

void oled_Fill(OLED_COLOR c) {
    memset(pixels, c == White, sizeof(pixels));
    screenText[0] = 0;
}
void oled_DrawPixel(uint8_t x, uint8_t y, OLED_COLOR c) {
    assert(x < OLED_WIDTH && y < OLED_HEIGHT);
    pixels[y][x] = c == White;
}
void oled_SetCursor(uint8_t x, uint8_t y) {
    assert(x < OLED_WIDTH && y + 10 <= OLED_HEIGHT);
}
char oled_WriteString(char *s, FontDef font, OLED_COLOR color) {
    (void)font; (void)color;
    assert(strlen(screenText) + strlen(s) + 2 < sizeof(screenText));
    strcat(screenText, s);
    strcat(screenText, "|");
    return 0;
}
void oled_UpdateScreen(void) { frames++; }
TickType_t xTaskGetTickCount(void) { return 123; }
void vTaskSuspend(void *p) { (void)p; assert(!"Unexpected game error"); }
TimerHandle_t xTimerCreate(const char *s, TickType_t t, BaseType_t b, void *p, void (*f)(TimerHandle_t)) {
    (void)s; (void)t; (void)b; (void)p; (void)f; return (void *)1;
}
BaseType_t xTimerStart(TimerHandle_t t, TickType_t d) { (void)t; (void)d; return pdPASS; }
BaseType_t xTimerStop(TimerHandle_t t, TickType_t d) { (void)t; (void)d; return pdPASS; }
void gameTickEvent(void) {}
void gameStartEvent(void) {}
void clearEvents(void) {}
void clearKeyQueue(void) {}
int readKeyFromQueue(void) { return EMPTY_QUEUE; }
EventGroupHandle_t getEvent(void) { return (void *)1; }
EventBits_t xEventGroupWaitBits(EventGroupHandle_t e, EventBits_t b, BaseType_t c, BaseType_t a, TickType_t t) {
    (void)e; (void)b; (void)c; (void)a; (void)t; return 0;
}

static void checkSnake(void) {
    int n = 0, occupied = 0;
    SnakeCell *previous = NULL;
    for (SnakeCell *cell = snake->head; cell; cell = cell->next) {
        assert(cell->prev == previous);
        assert(FieldGet(cell->pos) == CELL_SNAKE);
        previous = cell;
        assert(++n <= FIELD_CELLS);
    }
    assert(previous == snake->tail && n == snake->length);
    for (int y = 0; y < FIELD_HEIGHT; y++)
        for (int x = 0; x < FIELD_WIDTH; x++)
            occupied += FieldGet((Position){x, y}) == CELL_SNAKE;
    assert(occupied == n);
}

static void setBody(const Position *positions, int count, Direction direction) {
    FieldInit();
    initAllocPool();
    snake->head = snake->tail = NULL;
    snake->length = count;
    snake->direction = snake->nextDirection = direction;
    for (int i = 0; i < count; i++) {
        SnakeCell *cell = allocSnake(positions[i]);
        assert(cell);
        cell->prev = snake->tail;
        if (snake->tail) snake->tail->next = cell;
        else snake->head = cell;
        snake->tail = cell;
        FieldSet(cell->pos, CELL_SNAKE);
    }
}

int main(void) {
    assert(comparePositions((Position){16, 7}, (Position){16, 7}));
    assert(!comparePositions((Position){3, 8}, (Position){3, 3}));
    FieldInit();
    Snake_Init(snake);
    for (int i = 0; i < FIELD_WIDTH * 3; i++) {
        assert(Snake_Move(snake));
        checkSnake();
    }
    snake->head->pos = (Position){0, 0};
    snake->nextDirection = DIR_UP;
    assert(getNewHeadPosition(snake).y == FIELD_HEIGHT - 1);
    snake->nextDirection = DIR_LEFT;
    assert(getNewHeadPosition(snake).x == FIELD_WIDTH - 1);
    snake->head->pos = (Position){FIELD_WIDTH - 1, FIELD_HEIGHT - 1};
    snake->nextDirection = DIR_RIGHT;
    assert(getNewHeadPosition(snake).x == 0);
    snake->nextDirection = DIR_DOWN;
    assert(getNewHeadPosition(snake).y == 0);

    FieldInit();
    Snake_Init(snake);
    Snake_SetNewDirection(snake, RIGHT_KEY);
    assert(snake->nextDirection == DIR_LEFT);
    Snake_SetNewDirection(snake, UP_KEY);
    Snake_SetNewDirection(snake, RIGHT_KEY);
    assert(snake->nextDirection == DIR_UP);

    /* Entering an apple must grow immediately, retaining the old tail. */
    enterAnyKey();
    StartGame();
    FieldSet(apple, CELL_EMPTY);
    apple = getNewHeadPosition(snake);
    FieldSet(apple, CELL_APPLE);
    Position oldTail = snake->tail->pos, eaten = apple;
    assert(!GameTick());
    assert(snake->length == 2);
    assert(comparePositions(snake->head->pos, eaten));
    assert(comparePositions(snake->tail->pos, oldTail));
    assert(FieldGet(apple) == CELL_APPLE);
    checkSnake();

    /* Tail is legal when moving, but not when growing. */
    const Position ring[] = {{1, 1}, {1, 2}, {2, 2}, {2, 1}};
    setBody(ring, 4, DIR_RIGHT);
    assert(!Snake_Grow(snake));
    assert(Snake_Move(snake));
    checkSnake();

    const Position body[] = {{1, 1}, {1, 2}, {2, 2}, {2, 1}, {3, 1}};
    setBody(body, 5, DIR_RIGHT);
    state = GAME_RUNNING;
    apple = (Position){8, 8};
    assert(GameTick());
    assert(state == GAME_OVER);
    checkSnake();

    /* Apple generator: one free cell, then no free cells. */
    for (int y = 0; y < FIELD_HEIGHT; y++)
        for (int x = 0; x < FIELD_WIDTH; x++)
            FieldSet((Position){x, y}, CELL_SNAKE);
    FieldSet((Position){7, 3}, CELL_EMPTY);
    Position result;
    assert(generateNewApplePosition(&result));
    assert(comparePositions(result, (Position){7, 3}));
    FieldSet(result, CELL_SNAKE);
    assert(!generateNewApplePosition(&result));

    /* A continuous serpentine body fills the board after the last apple. */
    Position positions[FIELD_CELLS];
    int count = 0;
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int i = 0; i < FIELD_WIDTH; i++) {
            int x = (y % 2) ? FIELD_WIDTH - 1 - i : i;
            if (x || y) positions[count++] = (Position){x, y};
        }
    }
    setBody(positions, count, DIR_LEFT);
    apple = (Position){0, 0};
    FieldSet(apple, CELL_APPLE);
    state = GAME_RUNNING;
    assert(GameTick() && state == GAME_WIN);
    assert(snake->length == FIELD_CELLS);
    assert(allocSnakeZeroPosition() == NULL);
    checkSnake();

    enterAnyKey();
    assert(snake->length == 1 && state == GAME_STARTING);
    assert(!GameTick()); /* No movement during countdown. */
    checkSnake();

    renderStartField();
    unsigned before = frames;
    renderUpdateScreen(2, 3, CELL_SNAKE);
    for (int y = 12; y < 16; y++)
        for (int x = 8; x < 12; x++) assert(pixels[y][x]);
    renderUpdateScreen(2, 3, CELL_APPLE);
    assert(!pixels[12][8] && pixels[12][10]);
    int lit = 0;
    for (int y = 12; y < 16; y++)
        for (int x = 8; x < 12; x++) lit += pixels[y][x];
    assert(lit == 9);
    renderUpdateScreen(2, 3, CELL_EMPTY);
    for (int y = 12; y < 16; y++)
        for (int x = 8; x < 12; x++) assert(!pixels[y][x]);
    assert(frames == before); /* Helpers never present partial frames. */
    renderEndGameMenu(WIN, FIELD_CELLS - 1);
    assert(frames == before + 1 && strstr(screenText, "You win!"));
    char expectedScore[24];
    snprintf(expectedScore, sizeof(expectedScore), "Score: %d", FIELD_CELLS - 1);
    assert(strstr(screenText, expectedScore));
    renderEndGameMenu(LOSE, 0);
    assert(strstr(screenText, "Game over") && strstr(screenText, "Score: 0"));
    renderError();
    renderStartMenu();
    puts("PASS: movement, wrapping, turns, growth, collisions, apples, victory, restart, rendering");
    return 0;
}
