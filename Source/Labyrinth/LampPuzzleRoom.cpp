// Fill out your copyright notice in the Description page of Project Settings.


#include "LampPuzzleRoom.h"

ALampPuzzleRoom::ALampPuzzleRoom() :nbLamps{4} {

}

void ALampPuzzleRoom::InitPuzzle(FRandomStream seed)
{
	Super::InitPuzzle(seed);
}

void ALampPuzzleRoom::CreateLamps(std::vector<LabBlock*> lamps, const TArray<ATile*>& tiles)
{

}
