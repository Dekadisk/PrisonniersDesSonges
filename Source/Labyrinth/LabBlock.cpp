#include "LabBlock.h"

float LabBlock::assetSize = 550.f;

void LabBlock::removeWalls(LabBlock& next) {
		int x = coordX - next.coordX;
		int y = coordY - next.coordY;
	
		if (x == 1) {
			walls[LabBlock::Direction::WEST] = false;
			neighbors[LabBlock::Direction::WEST] = &next;
			next.walls[Direction::EAST] = false;
			next.neighbors[LabBlock::Direction::EAST] = this;
		}
		else if (x == -1) {
			walls[Direction::EAST] = false;
			neighbors[Direction::EAST] = &next;
			next.walls[LabBlock::Direction::WEST] = false;
			next.neighbors[LabBlock::Direction::WEST] = this;
		}
		else if (y == 1) {
			walls[Direction::NORTH] = false;
			neighbors[Direction::NORTH] = &next;
			next.walls[Direction::SOUTH] = false;
			next.neighbors[Direction::SOUTH] = this;
		}
		else if (y == -1) {
			walls[Direction::SOUTH] = false;
			neighbors[Direction::SOUTH] = &next;
			next.walls[Direction::NORTH] = false;
			next.neighbors[Direction::NORTH] = this;
		}
}