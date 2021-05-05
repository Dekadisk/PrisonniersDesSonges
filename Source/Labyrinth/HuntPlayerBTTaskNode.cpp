#include "HuntPlayerBTTaskNode.h"
#include "AIEnemyController.h"

UHuntPlayerBTTaskNode::UHuntPlayerBTTaskNode()
{
	// Le nom que prendra le noeud dans le BT
	NodeName = "HuntPlayer";

	// Cette t�che appelle TickTask
	bNotifyTick = true;
}

/* Sera appel�e au d�marrage de la t�che et devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UHuntPlayerBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*
	NodeMemory)
{
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	// Nous pr�parons le r�sultat de la t�che. Elle doit retourner InProgress
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
/* Sera appel�e constamment tant que la t�che n'est pas finie (tant que ExecuteTask retourne
InProgress) */
void UHuntPlayerBTTaskNode::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	// Appeler la fonction MoveToEnemy du contr�leur et nous conservons le r�sultat
	 // en MoveToActorResult
	EPathFollowingRequestResult::Type HuntPlayerResult = AIEnemyController->MoveToPlayer();

	// Si nous sommes rendu sur l'objectif, nous terminons la t�che avec succ�s
	if (HuntPlayerResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

/** Retourne une chaine de description pour la t�che. Ce texte appara�tre dans le BT */
FString UHuntPlayerBTTaskNode::GetStaticDescription() const
{
	return TEXT("Prise en chasse du joueur.");
}

void UHuntPlayerBTTaskNode::HuntAgain() {

	startHunt = true;
}