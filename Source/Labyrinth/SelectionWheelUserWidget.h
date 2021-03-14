// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionWheelUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API USelectionWheelUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativeOnInitialized() override;

	FReply NativeOnMouseMove
	(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent
	) override;

private:
	float LocationX;
	float LocationY;

	int32 sizeX;
	int32 sizeY;

	int32 cX;
	int32 cY;

	FVector2D vert;
	int32 xvertical;
	int32 yvertical;
};
