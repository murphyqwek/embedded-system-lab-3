#ifndef INC_SNAKE_RENDER_H_
#define INC_SNAKE_RENDER_H_

#include "snake/game_types.h"

void renderStartMenu(void);
void renderStartField(void);
void renderUpdateScreen(int x, int y, CellType cell);
void renderScore(int score);
void renderEndGameMenu(EndGameState state, int score);

void renderError(void);

#endif
