#include "snake/snake.h"
#include "snake/snake_alloc.h"
#include "snake/field.h"
#include <stddef.h>

static SnakePool snakePool;

/*
 * Чтобы не заставлять контроллер аллоцировать новые головы каждый раз (ну и не следить
 * за их очищением) был реализован alloc pool. Все головы уже созданы, мы просто выдаем
 * головы из этого пула
 */

void initAllocPool(void) {
	snakePool.nextFree = 0;
}

SnakeCell *allocSnake(Position pos) {
	if(snakePool.nextFree >= FIELD_CELLS) {
		return NULL;
	}

	SnakeCell *cell = &snakePool.cells[snakePool.nextFree];
	snakePool.nextFree++;

	cell->next = NULL;
	cell->prev = NULL;

	cell->pos = pos;

	return cell;

}
SnakeCell *allocSnakeZeroPosition() {
	return allocSnake((Position){0, 0});
}
