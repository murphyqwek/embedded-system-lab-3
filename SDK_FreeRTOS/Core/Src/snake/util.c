#include "snake/util.h"

bool comparePositions(Position pos1, Position pos2) {
	return pos1.x == pos2.x && pos1.y == pos2.y;
}
