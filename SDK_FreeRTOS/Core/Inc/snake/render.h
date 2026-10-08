#ifndef INC_SNAKE_RENDER_H_
#define INC_SNAKE_RENDER_H_

#include "snake/game_types.h"

void renderStartMenu(void);
void renderStartField(void);
void renderUpdateScreen(int x, int y, CellType cell);
void renderEndGameMenu(EndGameState state);

//Нужно написать функцию вывода ошибки
void renderError();

#endif
