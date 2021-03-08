// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class LABYRINTH_API ResponseCurve
{
public:
	enum class CurveType {
		Quadratic,
		Logistic
	};

public:
	static float Calculate(CurveType type, float x, float m, float b, float c, float k);
};
