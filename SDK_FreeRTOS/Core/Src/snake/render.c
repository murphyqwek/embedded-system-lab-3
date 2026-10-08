#include "oled.h"
#include "snake/render.h"
#include "snake/field.h"
#include <stdio.h>

#define CELL_SIZE 4
#define SCORE_Y (FIELD_HEIGHT * CELL_SIZE + 4)

_Static_assert(FIELD_WIDTH * CELL_SIZE <= OLED_WIDTH, "Field too wide");
_Static_assert(SCORE_Y + 10 <= OLED_HEIGHT, "No room for score");

static void drawApple(int px, int py) {
    static const uint8_t sprite[4] = {0x2, 0x6, 0xF, 0x6};
    for (int y = 0; y < CELL_SIZE; y++) {
        for (int x = 0; x < CELL_SIZE; x++) {
            oled_DrawPixel(px + x, py + y,
                (sprite[y] & (1U << (3 - x))) ? White : Black);
        }
    }
}

static void drawFullSquare(int px, int py, OLED_COLOR color) {
    for (int y = 0; y < CELL_SIZE; y++) {
        for (int x = 0; x < CELL_SIZE; x++) {
            oled_DrawPixel(px + x, py + y, color);
        }
    }
}

void renderStartMenu(void) {
    oled_Fill(Black);
    oled_SetCursor(0, 0);
    oled_WriteString("Snake", Font_7x10, White);
    oled_SetCursor(0, 14);
    oled_WriteString("2: up  8: down", Font_7x10, White);
    oled_SetCursor(0, 28);
    oled_WriteString("4: left 6: right", Font_7x10, White);
    oled_SetCursor(0, 48);
    oled_WriteString("Press any key", Font_7x10, White);
    oled_UpdateScreen();
}

/* Prepare the buffer; the caller draws cells before presenting it. */
void renderStartField(void) {
    oled_Fill(Black);
}

void renderUpdateScreen(int x, int y, CellType cell) {
    if (x < 0 || x >= FIELD_WIDTH || y < 0 || y >= FIELD_HEIGHT) return;
    int px = x * CELL_SIZE;
    int py = y * CELL_SIZE;
    switch (cell) {
        case CELL_APPLE:
            drawApple(px, py);
            break;
        case CELL_SNAKE:
            drawFullSquare(px, py, White);
            break;
        default:
            drawFullSquare(px, py, Black);
            break;
    }
}

void renderScore(int score) {
    char text[24];
    snprintf(text, sizeof(text), "Score: %d", score);
    for (int y = SCORE_Y; y < OLED_HEIGHT; y++) {
        for (int x = 0; x < OLED_WIDTH; x++) {
            oled_DrawPixel(x, y, Black);
        }
    }
    oled_SetCursor(0, SCORE_Y);
    oled_WriteString(text, Font_7x10, White);
}

void renderEndGameMenu(EndGameState state, int score) {
    char text[24];
    snprintf(text, sizeof(text), "Score: %d", score);
    oled_Fill(Black);
    oled_SetCursor(0, 0);
    oled_WriteString(state == WIN ? "You win!" : "Game over", Font_7x10, White);
    oled_SetCursor(0, 20);
    oled_WriteString(text, Font_7x10, White);
    oled_SetCursor(0, 42);
    oled_WriteString("Press any key", Font_7x10, White);
    oled_UpdateScreen();
}

void renderError(void) {
    oled_Fill(Black);
    oled_SetCursor(0, 0);
    oled_WriteString("Game error", Font_7x10, White);
    oled_SetCursor(0, 20);
    oled_WriteString("Reset the board", Font_7x10, White);
    oled_UpdateScreen();
}
