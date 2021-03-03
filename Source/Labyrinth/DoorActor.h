// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "DoorActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ADoorActor : public AUsableActor
{
	GENERATED_BODY()
public:
	ADoorActor();
public:
	UPROPERTY(EditAnywhere, Category = "Etat", Replicated)
	bool bIsLocked;
	UPROPERTY(EditAnywhere, Category = "Etat", Replicated)
	bool bIsOpen;

	virtual void OnUsed(AActor* InstigatorActor) override;
	virtual void Tick(float DeltaTime) override;

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

private:
	float fOpenPercent;
	float fSpeedOpen;
};
