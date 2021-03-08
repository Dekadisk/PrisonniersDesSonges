// Fill out your copyright notice in the Description page of Project Settings.


#include "HintDecalActor.h"
#include "PuzzleActor.h"
#include "SolvableActor.h"

void AHintDecalActor::Erase()
{
	isErased = true;
	SetDecalMaterial(erased);
	puzzleActor->targetActor[0].linkedActor->hasErasedHint = true;
}
