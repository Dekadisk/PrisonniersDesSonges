// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyController.h"

#include "EngineUtils.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"
#include "AIEnemyTargetPoint.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "LabCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AAIEnemyController::AAIEnemyController()
{
}

/** Sera utilisé par la tâche UpdateNextTargetPointBTTaskNode du
 Behavior Tree pour actualiser le chemin de patrouille */
void AAIEnemyController::UpdateNextTargetPoint()
{
	// Obtenir la référence au composant Blackbord de AIController
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	// Nous récupérons dans TargetPointNumber la valeur de la clé TargetPointNumber du Blackboard
	// Ce nombre représente l'ordre de parcours du chemin de patrouille
	int32 TargetPointNumber = BlackboardComponent->GetValueAsInt("TargetPointNumber");
	// Comme nous n'avons que 4 TargetPoint, quand nous sommes rendus au dernier,
	// nous revenons au premier.
	if (TargetPointNumber >= 4)
	{
		// Modifier aussi la valeur de la clé TargetPointNumber du Blackboard
		TargetPointNumber = 0;
		BlackboardComponent->SetValueAsInt("TargetPointNumber", TargetPointNumber);
	}
	// Pour tous les AAIEnemyTargetPointCpp du niveau
	for (TActorIterator<AAIEnemyTargetPoint> It(GetWorld()); It; ++It)
	{
		// Le TargetPoint à traiter
		AAIEnemyTargetPoint* TargetPoint = *It;
		// Si la clé TargetPointNumber du Blackboard est égale à l'attribut Position du point à traiter
		// Ce sera le point suivant dans le chemin du NPC et nous modifierons la clé TargetPointPosition
		// du blackboard avec la position de cet acteur.
		if (TargetPointNumber == TargetPoint->Position)
		{
			// La clé TargetPointPosition prend la valeur de la position de ce TargetPoint du niveau
			// Nous pouvons donc faire «break»
			BlackboardComponent->SetValueAsVector("TargetPointPosition", TargetPoint->GetActorLocation());
			break;
		}
	}
	// Finalement nous incrémentons la valeur de TargetPointNumber (dans le Blackboard)
	BlackboardComponent->SetValueAsInt("TargetPointNumber", (TargetPointNumber + 1));
}

/**
* Nous vérifions si le personnage est près et si c'est le cas, nous plaçons
* une référence dans le BlackBoard. Cette fonction sera utilisée par le service
* CheckNearbyEnemyBTService du BT pour implanter la vigilance du NPC lorsque
* nous approchons de sa zone de patrouille.
*/
void AAIEnemyController::CheckNearbyEnemy()
{
	// Nous obtenons un pointeur sur le pion du NPC
	APawn* PawnUsed = GetPawn();
	// Start: La position du NPC
	FVector MultiSphereStart = PawnUsed->GetActorLocation();
	// Nous créons un vecteur à partir de la position du NPC + 15 unités en Z.
	// Ce sont les deux valeurs qui serviront comme points de départ pour MultiSphereTraceForObjects
	FVector MultiSphereEnd = MultiSphereStart + FVector(0, 0, 15.0f);
	// Nous créons un tableau que nous utiliserons comme paramètre ObjectTypes dans
	// MultiSphereTraceForObjects, nous y définissons les types d'objets dont il faut tenir compte.
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	// Création d'un tableau d'acteurs à ignorer, pour l'instant seul le pion du NPC y sera.
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PawnUsed);
	// OutHits est un paramètre de sortie. La fonction SphereTraceMultiForObjects y placera
	// des objets de type FHitResult qui correspondent aux objets rencontrés.
	TArray<FHitResult> OutHits;
	bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), // Le monde
	MultiSphereStart, // Point de départ de la ligne qui défini la multisphère
	MultiSphereEnd, // Fin de la ligne qui défini la multisphère
	700, // Rayon de la sphère
	ObjectTypes, // Types d'objets dont il faut tenir compte
	false, // false car nous n'utilisons pas le mode complexe
	ActorsToIgnore, // Acteurs à ignorer
	EDrawDebugTrace::ForDuration, // Le type de Debug
	OutHits, // Où seront stockés les résultats
	true); // true s'il faut ignorer l'objet lui-même
	// La clé TargetActorToFollow est initialisée à NULL pour le cas où il n'y aurait
		// pas d'objet dans la multisphère.
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	BlackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);
	// Si nous avons des résultats
	if (Result == true)
	{
		// Nous parcourons tous les objets de OutHits
		for (int32 i = 0; i < OutHits.Num(); i++)
		{
			// Obtenir le FHitResult courant
			FHitResult Hit = OutHits[i];
			// Référence à notre personnage - joueur
			AActor* CharacterUsed = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			// Si l'acteur détecté est le joueur:
			// nous modifions la clé TargetActorToFollow du Blackboard avec un pointeur sur le joueur
			if (Hit.GetActor() == CharacterUsed)
			{
				BlackboardComponent->SetValueAsObject("TargetActorToFollow", CharacterUsed);
			}
		}
	}
}

void AAIEnemyController::CheckNearbyEnemyRay()
{
	APawn* PawnUsed = GetPawn();

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ALabCharacter* PlayerLabCharacter = Cast<ALabCharacter>(PlayerCharacter);
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();

	if (PlayerLabCharacter != nullptr)
		return;

	if (PlayerLabCharacter->IsHidden())
		BlackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);
	else {
		FVector PositionAI = PawnUsed->GetActorLocation();
		FVector PositionPlayer = PlayerLabCharacter->GetActorLocation();

		FVector DistanceBetween = PositionPlayer - PositionAI;
		DistanceBetween.FVector::Normalize();

		FVector FVectorAI = PawnUsed->GetActorForwardVector();

		float DotProd = FVector::DotProduct(DistanceBetween, FVectorAI);

		if (UKismetMathLibrary::DegAcos(DotProd) < 90.0f)
		{
			AActor* AIActor = GetOwner();
			
			FCollisionQueryParams TraceParams(FName(TEXT("TraceAI2Player")), true, this);
			TraceParams.bReturnPhysicalMaterial = false;
			TraceParams.bTraceComplex = true;

			FHitResult Hit(ForceInit);
			GetWorld()->LineTraceSingleByChannel(Hit, PositionAI, PositionPlayer, ECC_Camera, TraceParams);
			ALabCharacter* CastedActor = Cast<ALabCharacter>(Hit.GetActor());
			if (CastedActor)
			{ 
				BlackboardComponent->SetValueAsObject("TargetActorToFollow", CastedActor);

				CastedActor->bNotSeenYet = false;
			}
			else
			{
				BlackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);
			}
		}

		else
			BlackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);
	}
	/*
	FVector MultiSphereStart = PawnUsed->GetActorLocation();
	FVector MultiSphereEnd = MultiSphereStart + FVector(0, 0, 15.0f);
	// Nous créons un tableau que nous utiliserons comme paramètre ObjectTypes dans
	// MultiSphereTraceForObjects, nous y définissons les types d'objets dont il faut tenir compte.
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	// Création d'un tableau d'acteurs à ignorer, pour l'instant seul le pion du NPC y sera.
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PawnUsed);
	// OutHits est un paramètre de sortie. La fonction SphereTraceMultiForObjects y placera
	// des objets de type FHitResult qui correspondent aux objets rencontrés.
	TArray<FHitResult> OutHits;
	bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), // Le monde
		MultiSphereStart, // Point de départ de la ligne qui défini la multisphère
		MultiSphereEnd, // Fin de la ligne qui défini la multisphère
		700, // Rayon de la sphère
		ObjectTypes, // Types d'objets dont il faut tenir compte
		false, // false car nous n'utilisons pas le mode complexe
		ActorsToIgnore, // Acteurs à ignorer
		EDrawDebugTrace::ForDuration, // Le type de Debug
		OutHits, // Où seront stockés les résultats
		true); // true s'il faut ignorer l'objet lui-même
		// La clé TargetActorToFollow est initialisée à NULL pour le cas où il n'y aurait
			// pas d'objet dans la multisphère.
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	BlackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);
	// Si nous avons des résultats
	if (Result == true)
	{
		// Nous parcourons tous les objets de OutHits
		for (int32 i = 0; i < OutHits.Num(); i++)
		{
			// Obtenir le FHitResult courant
			FHitResult Hit = OutHits[i];
			// Référence à notre personnage - joueur
			AActor* CharacterUsed = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			// Si l'acteur détecté est le joueur:
			// nous modifions la clé TargetActorToFollow du Blackboard avec un pointeur sur le joueur
			if (Hit.GetActor() == CharacterUsed)
			{
				BlackboardComponent->SetValueAsObject("TargetActorToFollow", CharacterUsed);
			}
		}
	}*/
}

EPathFollowingRequestResult::Type AAIEnemyController::MoveToEnemy()
{
	// Obtenir un pointeur sur le blackboard
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	// Obtenir un pointeur sur le personnage référencé par la clé TargetActorToFollow du BlackBoard
	AActor* HeroCharacterActor = Cast<AActor>(
		BlackboardComponent->GetValueAsObject("TargetActorToFollow"));
	// Démarrer le processus de poursuite du personnage
	EPathFollowingRequestResult::Type MoveToActorResult = MoveToActor(HeroCharacterActor);
	return MoveToActorResult;
}
