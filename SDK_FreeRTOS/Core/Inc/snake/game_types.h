#ifndef POSITION_H
#define POSITION_H

#define UP_KEY 2
#define DOWN_KEY 8
#define LEFT_KEY 4
#define RIGHT_KEY 6

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

typedef enum {
	GAME_START_MENU,
	GAME_STARTING,
	GAME_RUNNING,
	GAME_OVER,
	GAME_WIN
} GameState;

typedef enum {
	WIN,
	LOSE
} EndGameState;

#endif
