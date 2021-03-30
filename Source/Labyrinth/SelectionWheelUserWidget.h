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

	void NativeConstruct() override;
	USelectionWheelUserWidget(const FObjectInitializer& ObjectInitializer);

	FReply NativeOnMouseMove
	(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent
	) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		float Ang{};

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureCircle;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureCross;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureQuestion;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureLeft;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureForward;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureRight;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureBase;

	UFUNCTION(BlueprintCallable)
		FSlateBrush UpdateImage();
	UFUNCTION(BlueprintCallable)
		bool GetHasMoved();
	

	float GetAngle();

private:


	bool bHasMoved{ false };

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
