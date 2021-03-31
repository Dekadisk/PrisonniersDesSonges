#include "ClearBlackboardBTService.h"
#include "AIEnemyController.h"

/** Constructeur de la classe */
UClearBlackboardBTService::UClearBlackboardBTService()
{
	// Nom du noeud dans le BT
	NodeName = "ClearBlackboard";

	// Intervalle de mise à jour
	Interval = 0.1f;
}

/** Sera appelée à chaque «update» du service */
void UClearBlackboardBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float
	DeltaSeconds)
{
	// Appeler la fonction de la classe de base
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	AIEnemyController->ClearBlackboard();
}