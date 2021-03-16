// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckElementBTService.h"
#include "AIEnemyController.h"

/** Constructeur de la classe */
UCheckElementBTService::UCheckElementBTService()
{
	// Nom du noeud dans le BT
	NodeName = "CheckElementChangedState";
	// Intervalle de mise � jour
	Interval = 0.1f;
}

/** Sera appel�e � chaque �update� du service */
void UCheckElementBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float
	DeltaSeconds)
{
	// Appeler la fonction de la classe de base
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());
	// Appeler la fonction CheckNearbyEnemy du AIEnemyController dans lequel nous avions
	// programm� la logique pour d�terminer si le personnage �tait autour et au besoin
	// pour modifier la cl� du blackboard en cons�quence.
	AIEnemyController->CheckElementChangedState();
}