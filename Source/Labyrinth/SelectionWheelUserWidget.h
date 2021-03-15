// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <UMG.h>
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

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		float Ang{};

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* selection_wheel_circle;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* selection_wheel_cross;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* selection_wheel_question;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* selection_wheel_left;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* selection_wheel_straight;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* selection_wheel_right;

	UFUNCTION(BlueprintCallable)
		FSlateBrush UpdateImage(float Angle);

	float GetAngle();

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
