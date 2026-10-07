#ifndef FIELD_H
#define FIELD_H
#include "snake/game_types.h"

#define FIELD_HEIGHT 14
#define FIELD_WIDTH 32

#define FIELD_CELLS (FIELD_HEIGHT * FIELD_WIDTH)

void FieldInit(void);
CellType FieldGet(Position pos);
void FieldSet(Position pos, CellType type);

int fillFreePositions(Position freePositions[]);

#endif
