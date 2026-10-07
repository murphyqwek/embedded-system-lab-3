#include "FreeRTOS.h"
#include "snake/game_types.h"
#include "snake/field.h"

static CellType field[FIELD_HEIGHT][FIELD_WIDTH];

void FieldInit(void) {
	for(int y = 0; y < FIELD_HEIGHT; y++) {
		for(int x = 0; x < FIELD_WIDTH; x++) {
			field[y][x] = CELL_EMPTY;
		}
	}
}

CellType FieldGet(Position pos) {
	return field[pos.y][pos.x];
}
void FieldSet(Position pos, CellType type) {
	field[pos.y][pos.x] = type;
	//TODO: не забыть обновить экран
}
