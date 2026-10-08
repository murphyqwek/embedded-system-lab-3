#include "snake/snake.h"
#include "snake/snake_alloc.h"
#include "snake/field.h"
#include "snake/game_types.h"
#include "snake/util.h"
#include <stddef.h>

void Snake_Init(Snake *snake) {
	initAllocPool();

	SnakeCell *cell = allocSnake((Position){FIELD_WIDTH / 2, FIELD_HEIGHT / 2});

	snake->length = 1;

	snake->direction = DIR_LEFT;
	snake->nextDirection = DIR_LEFT;

	snake->head = cell;
	snake->tail = cell;

	FieldSet(cell->pos, CELL_SNAKE);
}

//Возвращает true если удалось переместить змейку без коллизии
//Голова не может удариться об хвост
//Возвращает false, если голова ударилась об тело
bool Snake_Move(Snake *snake) {
	Position newHeadPosition = getNewHeadPosition(snake);

	if(Snake_CheckCollisionWithoutTail(snake, newHeadPosition)) {
		return false;
	}

	SnakeCell *tail = snake->tail;

	FieldSet(tail->pos, CELL_EMPTY);

	tail->pos = newHeadPosition;

	FieldSet(newHeadPosition, CELL_SNAKE);

	snake->direction = snake->nextDirection;

	if(snake->head == snake->tail) {
		return true;
	}

	// Отцепляем старый хвост
	snake->tail = tail->prev;
	snake->tail->next = NULL;

	// Настраиваем новую голову: ссылка next на старую голову, prev - NULL
	tail->next = snake->head;
	tail->prev = NULL;

	//Старая голова prev - на новую голову и устанавливаем новую голову
	snake->head->prev = tail;
	snake->head = tail;

	return true;
}

//Возвращает true, если удалось увеличить змейку без колизии
//Возвращает false, если произошла коллизия или не удалось выделить новую клетку змейки
bool Snake_Grow(Snake *snake) {
	Position newHeadPosition = getNewHeadPosition(snake);

	if(Snake_CheckCollision(newHeadPosition) == true) {
		return false;
	}

	SnakeCell *newHead = allocSnake(newHeadPosition);

	if(newHead == NULL) {
		return false;
	}

	snake->length++;

	snake->direction = snake->nextDirection;

	newHead->next = snake->head;
	newHead->prev = NULL;

	snake->head->prev = newHead;

	snake->head = newHead;

	FieldSet(newHeadPosition, CELL_SNAKE);

	return true;
}

bool Snake_CheckCollisionWithoutTail(Snake *snake, Position newHeadPosition) {
	if(comparePositions(snake->tail->pos, newHeadPosition)) {
		return false;
	}

	return FieldGet(newHeadPosition) == CELL_SNAKE;
}

bool Snake_CheckCollision(Position newHeadPosition) {
	return FieldGet(newHeadPosition) == CELL_SNAKE;
}

Position getNewHeadPosition(Snake *snake) {
	Position newPos = snake->head->pos;

	Direction dir = snake->nextDirection;

	switch(dir) {
		case DIR_UP:
			newPos.y--;
			break;
		case DIR_DOWN:
			newPos.y++;
			break;
		case DIR_LEFT:
			newPos.x--;
			break;
		case DIR_RIGHT:
			newPos.x++;
			break;
	}

	if(newPos.y < 0) newPos.y = FIELD_HEIGHT - 1;
	if(newPos.y >= FIELD_HEIGHT) newPos.y = 0;

	if(newPos.x < 0) newPos.x = FIELD_WIDTH - 1;
	if(newPos.x >= FIELD_WIDTH) newPos.x = 0;

	return newPos;
}


void Snake_SetNewDirection(Snake *snake, int key) {
	switch(key) {
		case UP_KEY:
			if(snake->direction != DIR_DOWN) snake->nextDirection = DIR_UP;
			break;
		case DOWN_KEY:
			if(snake->direction != DIR_UP) snake->nextDirection = DIR_DOWN;
			break;
		case LEFT_KEY:
			if(snake->direction != DIR_RIGHT) snake->nextDirection = DIR_LEFT;
			break;
		case RIGHT_KEY:
			if(snake->direction != DIR_LEFT) snake->nextDirection = DIR_RIGHT;
			break;
	}
}
