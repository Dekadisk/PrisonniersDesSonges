// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"


/**
 * 
 */
class LABYRINTH_API LabBlock
{
public:
	enum Direction { NORTH, SOUTH, EAST, WEST };

private:

	bool walls[4] = { true, true, true, true };
	LabBlock* neighbors[4]{ nullptr,nullptr ,nullptr ,nullptr };
	bool isVisited{ false };
	bool isLocked{ false };
	bool hasDoor{ false };
	bool hasKey{ false };
	bool hasHint{};
	int coordX{};
	int coordY{};
	int index{};
	int hintClockDirection{};

public:
	static float assetSize;

	LabBlock(int _x, int _y, int _index) : coordX{ _x }, coordY{ _y }, index{_index} {
	}
	LabBlock() = default;

	int GetX() { return coordX; };
	int GetY() { return coordY; };
	int GetIndex() { return index; };
	bool* GetWalls() { return walls; };
	LabBlock** GetNeighbors() { return neighbors; }

	bool IsWallNorth() { return walls[Direction::NORTH]; };
	bool IsWallSouth() { return walls[Direction::SOUTH]; };
	bool IsWallEast() { return walls[Direction::EAST]; };
	bool IsWallWest() { return walls[Direction::WEST]; };

	LabBlock* GetNeighborNorth() { return neighbors[Direction::NORTH]; };
	LabBlock* GetNeighborSouth() { return neighbors[Direction::SOUTH]; };
	LabBlock* GetNeighborEast() { return neighbors[Direction::EAST]; };
	LabBlock* GetNeighborWest() { return neighbors[Direction::WEST]; };

	void SetWallNorth(bool isThereWall) { walls[Direction::NORTH] = isThereWall; };
	void SetWallSouth(bool isThereWall) { walls[Direction::SOUTH] = isThereWall; };
	void SetWallEast(bool isThereWall) { walls[Direction::EAST] = isThereWall; };
	void SetWallWest(bool isThereWall) { walls[Direction::WEST] = isThereWall; };
	void SetWallsToVoid() {
		walls[Direction::NORTH] = true;
		walls[Direction::SOUTH] = true;
		walls[Direction::EAST] = true;
		walls[Direction::WEST] = true;
	}
	void SetNeighborNorth(LabBlock* neighbor) { neighbors[Direction::NORTH] = neighbor; };
	void SetNeighborSouth(LabBlock* neighbor) { neighbors[Direction::SOUTH] = neighbor; };
	void SetNeighborEast(LabBlock* neighbor) { neighbors[Direction::EAST] = neighbor; };
	void SetNeighborWest(LabBlock* neighbor) { neighbors[Direction::WEST] = neighbor; };
	void SetNeighborsToVoid() {
		neighbors[Direction::NORTH] = nullptr;
		neighbors[Direction::SOUTH] = nullptr;
		neighbors[Direction::EAST]  = nullptr;
		neighbors[Direction::WEST]  = nullptr;
	}
	void removeWalls(LabBlock& next);

	bool getVisited() { return isVisited; };
	void setVisited(bool visite) { isVisited = visite; };


	bool IsLocked() { return isLocked; }
	void setLocked(bool _isLocked) { isLocked = _isLocked; }

	bool GetHasDoor() { return hasDoor; }
	void SetHasDoor(bool _hasDoor) { hasDoor = _hasDoor; }

	bool GetHasKey() { return hasKey; }
	void SetHasKey(bool _hasKey) { hasKey = _hasKey; }

	bool GetHasHint() { return hasHint; }
	void SetHasHint(bool _hasHint) { hasHint = _hasHint; }
	FVector GetGlobalPos() { return {-GetX()*assetSize,-GetY()*assetSize,0 }; }

};
