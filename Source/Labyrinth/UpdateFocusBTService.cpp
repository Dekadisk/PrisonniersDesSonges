#include "UpdateFocusBTService.h"
#include "AIEnemyController.h"

/** Constructeur de la classe */
UUpdateFocusBTService::UUpdateFocusBTService()
{
	// Nom du noeud dans le BT
	NodeName = "UpdateFocus";

	// Intervalle de mise à jour
	Interval = 0.1f;
	
	RandomDeviation = 0.0f;
}

/** Sera appelée à chaque «update» du service */
void UUpdateFocusBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Appeler la fonction de la classe de base
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	AIEnemyController->UpdateFocus();
}
