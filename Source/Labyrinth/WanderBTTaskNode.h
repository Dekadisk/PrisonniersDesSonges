// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "WanderBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UWanderBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UWanderBTTaskNode();
	/* Fonction d'ex�cution de la t�che.
	Retournera Succeeded, Failed ou InProgress */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
		override;
	/** Permet de d�finir une description pour la t�che. C'est ce texte qui
	 appara�tra dans le noeud que nous ajouterons au Behavior Tree */
	virtual FString GetStaticDescription() const override;
};