#ifndef POSITION_H
#define POSITION_H

typedef struct {
	int x;
	int y;
} Position;

typedef enum {
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
} Direction;

typedef enum {
	CELL_EMPTY,
	CELL_SNAKE,
	CELL_APPLE
} CellType;

#endif
