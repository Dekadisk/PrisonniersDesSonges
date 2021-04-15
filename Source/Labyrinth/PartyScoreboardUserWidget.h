// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartyScoreboardUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UPartyScoreboardUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPartyScoreboardUserWidget(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureChalk;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureLantern;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureBackground;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureKey;
};
