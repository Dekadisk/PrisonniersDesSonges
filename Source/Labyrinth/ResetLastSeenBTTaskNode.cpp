// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetLastSeenBTTaskNode.h"
#include "AIEnemyController.h"

UResetLastSeenBTTaskNode::UResetLastSeenBTTaskNode()
{
	// Le nom que prendra le noeud dans le BT
	NodeName = "ResetLastSeen";
}
/* Sera appel�e au d�marrage de la t�che et devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UResetLastSeenBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*
	NodeMemory)
{
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	AIEnemyController->ResetLastSeen();

	return EBTNodeResult::Succeeded;
}

/** Retourne une chaine de description pour la t�che. Ce texte appara�tre dans le BT */
FString UResetLastSeenBTTaskNode::GetStaticDescription() const
{
	return TEXT("Reset du nombre de secondes depuis la vue du joueur.");
}

