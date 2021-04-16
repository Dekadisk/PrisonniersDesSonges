// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackPlayerBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UAttackPlayerBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UAttackPlayerBTTaskNode();

	/* Sera appelée au démarrage de la tâche et devra retourner Succeeded, Failed ou InProgress */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** Retourne une chaine de description pour la tâche. Ce texte apparaître dans le BT */
	virtual FString GetStaticDescription() const override;
};
