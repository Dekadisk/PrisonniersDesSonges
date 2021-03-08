// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckNearbyEnemyBTService.h"
#include "AIEnemyController.h"


/** Constructeur de la classe */
UCheckNearbyEnemyBTService::UCheckNearbyEnemyBTService()
{
	// Nom du noeud dans le BT
	NodeName = "CheckNearbyEnemy";
	// Intervalle de mise à jour
	Interval = 0.1f;
	// Valeur aléatoire de déviation de l'intervalle de mise à jour
	RandomDeviation = 0.05f;
}

/** Sera appelée à chaque «update» du service */
void UCheckNearbyEnemyBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float
	DeltaSeconds)
{
	// Appeler la fonction de la classe de base
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());
	// Appeler la fonction CheckNearbyEnemy du AIEnemyController dans lequel nous avions
	// programmé la logique pour déterminer si le personnage était autour et au besoin
	// pour modifier la clé du blackboard en conséquence.
	//AIEnemyController->CheckNearbyEnemy();
}
