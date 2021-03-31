// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeZoneBTTaskNode.h"
#include "AIEnemyController.h"

UChangeZoneBTTaskNode::UChangeZoneBTTaskNode()
{
	// Le nom que prendra le noeud dans le BT
	NodeName = "ChangeZone";
	// Cette tâche appelle TickTask
	bNotifyTick = true;
}
/* Sera appelée au démarrage de la tâche et devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UChangeZoneBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*
	NodeMemory)
{
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());
	// Nous préparons le résultat de la tâche. Elle doit retourner InProgress
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	// Appel de la fonction MoveToEnemy du contrôleur, nous plaçons le résultat dans MoveToActorResult
	EPathFollowingRequestResult::Type MoveToActorResult = AIEnemyController->ChangeZone();
	// Si nous sommes déjà auprès du personnage. Nous retournons Succeeded.
	if (MoveToActorResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		NodeResult = EBTNodeResult::Succeeded;
	}
	return NodeResult;
}
/* Sera appelée constamment tant que la tâche n'est pas finie (tant que ExecuteTask retourne
InProgress) */
void UChangeZoneBTTaskNode::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());
	// Appeler la fonction MoveToEnemy du contrôleur et nous conservons le résultat
	 // en MoveToActorResult
	EPathFollowingRequestResult::Type MoveToActorResult = AIEnemyController->ChangeZone();
	// Si nous sommes rendu sur l'objectif, nous terminons la tâche avec succès
	if (MoveToActorResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
/** Retourne une chaine de description pour la tâche. Ce texte apparaître dans le BT */
FString UChangeZoneBTTaskNode::GetStaticDescription() const
{
	return TEXT("Changement de zone.");
}