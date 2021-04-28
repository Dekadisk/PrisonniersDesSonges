#include "MoveToPriorityBTTaskNode.h"
#include "AIEnemyController.h"

UMoveToPriorityBTTaskNode::UMoveToPriorityBTTaskNode()
{
	// Le nom que prendra le noeud dans le BT
	NodeName = "MoveToPriorityPoint";

	// Cette tâche appelle TickTask
	bNotifyTick = true;
}

/* Sera appelée au démarrage de la tâche et devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UMoveToPriorityBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	// Nous préparons le résultat de la tâche. Elle doit retourner InProgress
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	EPathFollowingRequestResult::Type MoveToPriorityResult = AIEnemyController->MoveToPriorityPoint();

	if (MoveToPriorityResult == EPathFollowingRequestResult::AlreadyAtGoal)
		NodeResult = EBTNodeResult::Succeeded;

	return NodeResult;
}
/* Sera appelée constamment tant que la tâche n'est pas finie (tant que ExecuteTask retourne
InProgress) */
void UMoveToPriorityBTTaskNode::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	// Appeler la fonction MoveToEnemy du contrôleur et nous conservons le résultat
	 // en MoveToActorResult
	EPathFollowingRequestResult::Type MoveToPriorityResult = AIEnemyController->MoveToPriorityPoint();

	// Si nous sommes rendu sur l'objectif, nous terminons la tâche avec succès
	if (MoveToPriorityResult == EPathFollowingRequestResult::AlreadyAtGoal)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

/** Retourne une chaine de description pour la tâche. Ce texte apparaître dans le BT */
FString UMoveToPriorityBTTaskNode::GetStaticDescription() const
{
	return TEXT("Deplacement vers un point designe par le director.");
}