#ifndef INC_SNAKE_SNAKE_ALLOC_H_
#define INC_SNAKE_SNAKE_ALLOC_H_

#include "snake/snake.h"
#include "game_types.h"
#include "snake/field.h"

typedef struct {
	SnakeCell cells[FIELD_CELLS];
	int nextFree;
} SnakePool;

SnakeCell *allocSnake(Position pos);
SnakeCell *allocSnakeZeroPosition(void);
void initAllocPool(void);

#endif
