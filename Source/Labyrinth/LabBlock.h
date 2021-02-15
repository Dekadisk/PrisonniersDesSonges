// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LABYRINTH_API LabBlock
{
public:
public:
	enum Direction { NORTH, SOUTH, EAST, WEST };

private:

	bool walls[4] = { true, true, true, true };
	bool isVisited{ false };
	int coordX{};
	int coordY{};

public:

	LabBlock(int _x, int _y) : coordX{ _x }, coordY{ _y } {
	}
	LabBlock() = default;

	int GetX() { return coordX; };
	int GetY() { return coordY; };
	bool* GetWalls() { return walls; };

	bool IsWallNorth() { return walls[Direction::NORTH]; };
	bool IsWallSouth() { return walls[Direction::SOUTH]; };
	bool IsWallEast() { return walls[Direction::EAST]; };
	bool IsWallWest() { return walls[Direction::WEST]; };

	void SetWallNorth(bool isThereWall) { walls[Direction::NORTH] = isThereWall; };
	void SetWallSouth(bool isThereWall) { walls[Direction::SOUTH] = isThereWall; };
	void SetWallEast(bool isThereWall) { walls[Direction::EAST] = isThereWall; };
	void SetWallWest(bool isThereWall) { walls[Direction::WEST] = isThereWall; };

	void removeWalls(LabBlock& next);

	bool getVisited() { return isVisited; };
	void setVisited(bool visite) { isVisited = visite; };

};
