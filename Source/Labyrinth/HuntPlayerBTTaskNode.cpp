#include "HuntPlayerBTTaskNode.h"
#include "AIEnemyController.h"

UHuntPlayerBTTaskNode::UHuntPlayerBTTaskNode()
{
	// Le nom que prendra le noeud dans le BT
	NodeName = "HuntPlayer";

	// Cette tâche appelle TickTask
	bNotifyTick = true;
}

/* Sera appelée au démarrage de la tâche et devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UHuntPlayerBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*
	NodeMemory)
{
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	// Nous préparons le résultat de la tâche. Elle doit retourner InProgress
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	GetWorld()->GetTimerManager().SetTimer(timerHuntHandle, this, &UHuntPlayerBTTaskNode::HuntAgain, AIEnemyController->MinHuntTime, false);

	if (startHunt) {
		startHunt = false;
		GetWorld()->GetTimerManager().ClearTimer(timerHuntHandle);
		AIEnemyController->StartHunt();
	}

	EPathFollowingRequestResult::Type HuntPlayerResult = AIEnemyController->MoveToPlayer();

	if (HuntPlayerResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		NodeResult = EBTNodeResult::Succeeded;
	}

	return NodeResult;
}
/* Sera appelée constamment tant que la tâche n'est pas finie (tant que ExecuteTask retourne
InProgress) */
void UHuntPlayerBTTaskNode::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	// Appeler la fonction MoveToEnemy du contrôleur et nous conservons le résultat
	 // en MoveToActorResult
	EPathFollowingRequestResult::Type HuntPlayerResult = AIEnemyController->MoveToPlayer();

	// Si nous sommes rendu sur l'objectif, nous terminons la tâche avec succès
	if (HuntPlayerResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

/** Retourne une chaine de description pour la tâche. Ce texte apparaître dans le BT */
FString UHuntPlayerBTTaskNode::GetStaticDescription() const
{
	return TEXT("Prise en chasse du joueur.");
}

void UHuntPlayerBTTaskNode::HuntAgain() {

	startHunt = true;
}