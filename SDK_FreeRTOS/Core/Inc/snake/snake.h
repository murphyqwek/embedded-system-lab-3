#ifndef INC_SNAKE_SNAKE_H_
#define INC_SNAKE_SNAKE_H_

#include "snake/game_types.h"
#include <stdbool.h>

typedef struct SnakeCell {
	Position pos;

	struct SnakeCell *prev;
	struct SnakeCell *next;
} SnakeCell;

typedef struct Snake {
	int length;

	Direction direction;
	Direction nextDirection;

	struct SnakeCell *head;
	struct SnakeCell *tail;
} Snake;

void Snake_Init(Snake *snake);

bool Snake_Move(Snake *snake);
bool Snake_Grow(Snake *snake);

bool Snake_CheckCollisionWithoutTail(Snake *snake, Position newHeadPosition);
bool Snake_CheckCollision(Position newHeadPosition);

Position getNewHeadPosition(Snake *snake);

void setNewDirection(Snake *snake, int key);

#endif
