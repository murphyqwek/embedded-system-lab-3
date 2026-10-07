#include "snake/field.h"
#include "snake/apple_generator.h"
#include <stdlib.h>

static Position freePositions[FIELD_CELLS];

bool generateNewApplePosition(Position *result) {
	int length = fillFreePositions(freePositions);

	if(length == 0) {
		return false;
	}

	int index = rand() % length;

	*result = freePositions[index];

	return true;
}
