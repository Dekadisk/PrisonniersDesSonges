// Fill out your copyright notice in the Description page of Project Settings.


#include "LabBlock.h"

void LabBlock::removeWalls(LabBlock& next) {
		int x = coordX - next.coordX;
		int y = coordY - next.coordY;
	
		if (x == 1) {
			walls[LabBlock::Direction::WEST] = false;
			next.walls[Direction::EAST] = false;
		}
		else if (x == -1) {
			walls[Direction::EAST] = false;
			next.walls[LabBlock::Direction::WEST] = false;
		}
		else if (y == 1) {
			walls[Direction::NORTH] = false;
			next.walls[Direction::SOUTH] = false;
		}
		else if (y == -1) {
			walls[Direction::SOUTH] = false;
			next.walls[Direction::NORTH] = false;
		}
}