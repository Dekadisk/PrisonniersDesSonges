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
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Runtime/NavigationSystem/Public/NavigationPath.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "LabyrinthGameStateBase.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"

#include <algorithm>

AAIEnemyController::AAIEnemyController()
{
	PlayerToFollow = nullptr;
}

void AAIEnemyController::BeginPlay() {
	Super::BeginPlay();
	AGameStateBase* GameState = GetWorld()->GetGameState<ALabyrinthGameStateBase>();
	TArray<APlayerState*> PlayerArray = GameState->PlayerArray;
	std::for_each(PlayerArray.begin(), PlayerArray.end(), [&](APlayerState* playerstate) {
		PlayersLastSeen[playerstate->GetUniqueID()] = 10.f;
		});
}

/** Sera utilis� par la t�che UpdateNextTargetPointBTTaskNode du
 Behavior Tree pour actualiser le chemin de patrouille */
void AAIEnemyController::UpdateNextTargetPoint()
{

	/*APawn* PawnUsed = GetPawn();

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ALabCharacter* LabCharacter = Cast<ALabCharacter>(PlayerCharacter);

	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	float SecondsSinceLastSeen = BlackboardComponent->GetValueAsFloat("SecondsSinceLastSeen");

	if (LabCharacter->bNotSeenYet || SecondsSinceLastSeen >= 5.0f) {

		int32 TargetPointNumber = BlackboardComponent->GetValueAsInt("TargetPointNumber");

		if (TargetPointNumber >= 10) // <---------------------------------------------------------------- A RENDRE DYNAMIQUE
		{
			TargetPointNumber = 0;
			BlackboardComponent->SetValueAsInt("TargetPointNumber", TargetPointNumber);
		}

		// Pour tous les AAIEnemyTargetPointCpp du niveau
		for (TActorIterator<AAIEnemyTargetPoint> It(GetWorld()); It; ++It)
		{
			// Le TargetPoint � traiter
			AAIEnemyTargetPoint* TargetPoint = *It;
			// Si la cl� TargetPointNumber du Blackboard est �gale � l'attribut Position du point � traiter
			// Ce sera le point suivant dans le chemin du NPC et nous modifierons la cl� TargetPointPosition
			// du blackboard avec la position de cet acteur.
			if (TargetPointNumber == TargetPoint->Position)
			{
				// La cl� TargetPointPosition prend la valeur de la position de ce TargetPoint du niveau
				// Nous pouvons donc faire �break�
				BlackboardComponent->SetValueAsVector("TargetPointPosition", TargetPoint->GetActorLocation());
				break;
			}
		}

		// Finalement nous incr�mentons la valeur de TargetPointNumber (dans le Blackboard)
		BlackboardComponent->SetValueAsInt("TargetPointNumber", (TargetPointNumber + 1));
	}
	else {

		float dist_min = INFINITY;
		AAIEnemyTargetPoint* point1{};
		AAIEnemyTargetPoint* point2{};

		for (TActorIterator<AAIEnemyTargetPoint> It(GetWorld()); It; ++It) {

			UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), It->GetActorLocation(), PlayerCharacter);

			if (!path->IsPartial()) {
				if (path->GetPathLength() < dist_min) {
					dist_min = path->GetPathLength();
					point1 = *It;
				}
			}

		}

		dist_min = INFINITY;

		for (TActorIterator<AAIEnemyTargetPoint> It(GetWorld()); It; ++It) {

			if (point1->Position != It->Position) {
				UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), It->GetActorLocation(), PlayerCharacter);

				if (!path->IsPartial()) {
					if (path->GetPathLength() < dist_min) {
						dist_min = path->GetPathLength();
						point2 = *It;
					}
				}
			}

		}

		// DEBUG LINE
		DrawDebugLine(GetWorld(), point1->GetActorLocation() + FVector{ 0.0f,0.0f,20.0f }, point2->GetActorLocation() + FVector{ 0.0f,0.0f,20.0f }, FColor{ 255,0,0 }, false, 5.f, (uint8)'\000', 1.f);

		UNavigationPath* path1 = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), point1->GetActorLocation(), PlayerCharacter);
		UNavigationPath* path2 = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), point2->GetActorLocation(), PlayerCharacter);

		AAIEnemyTargetPoint* pointChoisi;

		if (path1->GetPathLength() < path2->GetPathLength()) {
			pointChoisi = point2;
		}
		else {
			pointChoisi = point1;
		}

		BlackboardComponent->SetValueAsInt("TargetPointNumber", pointChoisi->Position);
		BlackboardComponent->SetValueAsVector("TargetPointPosition", pointChoisi->GetActorLocation());

	}*/

	APawn* PawnUsed = GetPawn();

	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();

	int32 TargetPointNumber = BlackboardComponent->GetValueAsInt("TargetPointNumber");

	if (TargetPointNumber >= 10) // <---------------------------------------------------------------- A RENDRE DYNAMIQUE
	{
		TargetPointNumber = 0;
		BlackboardComponent->SetValueAsInt("TargetPointNumber", TargetPointNumber);
	}

	// Pour tous les AAIEnemyTargetPointCpp du niveau
	for (TActorIterator<AAIEnemyTargetPoint> It(GetWorld()); It; ++It)
	{
		// Le TargetPoint � traiter
		AAIEnemyTargetPoint* TargetPoint = *It;
		// Si la cl� TargetPointNumber du Blackboard est �gale � l'attribut Position du point � traiter
		// Ce sera le point suivant dans le chemin du NPC et nous modifierons la cl� TargetPointPosition
		// du blackboard avec la position de cet acteur.
		if (TargetPointNumber == TargetPoint->Position)
		{
			// La cl� TargetPointPosition prend la valeur de la position de ce TargetPoint du niveau
			// Nous pouvons donc faire �break�
			BlackboardComponent->SetValueAsVector("TargetPointPosition", TargetPoint->GetActorLocation());
			break;
		}
	}

	// Finalement nous incr�mentons la valeur de TargetPointNumber (dans le Blackboard)
	BlackboardComponent->SetValueAsInt("TargetPointNumber", (TargetPointNumber + 1));
		
}

/**
* Nous v�rifions si le personnage est pr�s et si c'est le cas, nous pla�ons
* une r�f�rence dans le BlackBoard. Cette fonction sera utilis�e par le service
* CheckNearbyEnemyBTService du BT pour implanter la vigilance du NPC lorsque
* nous approchons de sa zone de patrouille.
*/
/*void AAIEnemyController::CheckNearbyEnemy()
{
	// Nous obtenons un pointeur sur le pion du NPC
	APawn* PawnUsed = GetPawn();
	// Start: La position du NPC
	FVector MultiSphereStart = PawnUsed->GetActorLocation();
	// Nous cr�ons un vecteur � partir de la position du NPC + 15 unit�s en Z.
	// Ce sont les deux valeurs qui serviront comme points de d�part pour MultiSphereTraceForObjects
	FVector MultiSphereEnd = MultiSphereStart + FVector(0, 0, 15.0f);
	// Nous cr�ons un tableau que nous utiliserons comme param�tre ObjectTypes dans
	// MultiSphereTraceForObjects, nous y d�finissons les types d'objets dont il faut tenir compte.
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	// Cr�ation d'un tableau d'acteurs � ignorer, pour l'instant seul le pion du NPC y sera.
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PawnUsed);
	// OutHits est un param�tre de sortie. La fonction SphereTraceMultiForObjects y placera
	// des objets de type FHitResult qui correspondent aux objets rencontr�s.
	TArray<FHitResult> OutHits;
	bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), // Le monde
	MultiSphereStart, // Point de d�part de la ligne qui d�fini la multisph�re
	MultiSphereEnd, // Fin de la ligne qui d�fini la multisph�re
	700, // Rayon de la sph�re
	ObjectTypes, // Types d'objets dont il faut tenir compte
	false, // false car nous n'utilisons pas le mode complexe
	ActorsToIgnore, // Acteurs � ignorer
	EDrawDebugTrace::ForDuration, // Le type de Debug
	OutHits, // O� seront stock�s les r�sultats
	true); // true s'il faut ignorer l'objet lui-m�me
	// La cl� TargetActorToFollow est initialis�e � NULL pour le cas o� il n'y aurait
		// pas d'objet dans la multisph�re.
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	BlackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);
	// Si nous avons des r�sultats
	if (Result == true)
	{
		// Nous parcourons tous les objets de OutHits
		for (int32 i = 0; i < OutHits.Num(); i++)
		{
			// Obtenir le FHitResult courant
			FHitResult Hit = OutHits[i];
			// R�f�rence � notre personnage - joueur
			AActor* CharacterUsed = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			// Si l'acteur d�tect� est le joueur:
			// nous modifions la cl� TargetActorToFollow du Blackboard avec un pointeur sur le joueur
			if (Hit.GetActor() == CharacterUsed)
			{
				BlackboardComponent->SetValueAsObject("TargetActorToFollow", CharacterUsed);
			}
		}
	}
}*/

void AAIEnemyController::CheckNearbyEnemyRay()
{
	/*APawn* PawnUsed = GetPawn();

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ALabCharacter* PlayerLabCharacter = Cast<ALabCharacter>(PlayerCharacter);
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();

	if (PlayerLabCharacter == nullptr)
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
			UCapsuleComponent* capsule = Cast<ACharacter>(PawnUsed)->GetCapsuleComponent();
			FCollisionQueryParams TraceParams(FName(TEXT("TraceAI2Player")), false, capsule->GetOwner());
			//TraceParams.bReturnPhysicalMaterial = false;
			//TraceParams.bTraceComplex = false;

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
	}*/

	APawn* PawnUsed = GetPawn();

	TArray<APlayerState*> PlayerCharacters = GetClosestCharacters();

	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();

	if (PlayerCharacters.Num() == 0 && PlayerToFollow == nullptr) {
		BlackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);
	}
	else if (PlayerCharacters.Num() == 0 && PlayerToFollow != nullptr) {
		auto iterable = PlayersLastSeen.find(PlayerToFollow->GetUniqueID());
		if (iterable != PlayersLastSeen.end() && iterable->second >= 5.0f) {
			BlackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);
		}
		else {
			BlackboardComponent->SetValueAsObject("TargetActorToFollow", PlayerToFollow);
		}
	}
	else if (PlayerCharacters.Num() != 0) {

		//ACharacter* PlayerCharacter = Cast<ACharacter>((*PlayerCharacters.begin())->GetPawn());
		//ALabCharacter* PlayerLabCharacter = Cast<ALabCharacter>(PlayerCharacter);

		int i = 0;
		for (auto it : PlayerCharacters) {
			++i;
			ALabCharacter* LabCharacter = Cast<ALabCharacter>(it->GetPawn());
			if (LabCharacter->IsHidden() && i == PlayerCharacters.Num()) {
				BlackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);
			}
			else if (!LabCharacter->IsHidden()) {
				BlackboardComponent->SetValueAsObject("TargetActorToFollow", LabCharacter);
				PlayerToFollow = LabCharacter;
				break;
			}
		}
	}

}

void AAIEnemyController::UpdateLastSeen(float DeltaSeconds) {

	std::for_each(PlayersLastSeen.begin(), PlayersLastSeen.end(), [&](std::pair<const uint32, float>& pairLS) {
		pairLS.second += DeltaSeconds;
	});
}

void AAIEnemyController::ResetLastSeen() {

	std::for_each(PlayersLastSeen.begin(), PlayersLastSeen.end(), [&](std::pair<const uint32,float>& pairLS) {
		pairLS.second = 0.0f;
	});

}

void AAIEnemyController::ResetPlayerLastSeen(uint32 id) {

	auto it = PlayersLastSeen.find(id);
	it->second = 0.0f;

}

EPathFollowingRequestResult::Type AAIEnemyController::MoveToEnemy()
{

	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();

	// Obtenir un pointeur sur le personnage r�f�renc� par la cl� TargetActorToFollow du BlackBoard
	AActor* PlayerActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActorToFollow"));

	ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerActor);

	TArray<APlayerState*> playerArray = GetClosestCharacters();

	auto it = playerArray.FindByKey(PlayerCharacter->GetPlayerState());

	if (it != nullptr) {

		ResetPlayerLastSeen(PlayerCharacter->GetUniqueID());
		// D�marrer le processus de poursuite du personnage
		EPathFollowingRequestResult::Type MoveToActorResult = MoveToActor(PlayerActor,-1,false);
		return MoveToActorResult;

	}
	else {

		float dist_min = INFINITY;
		AAIEnemyTargetPoint* point1{};
		AAIEnemyTargetPoint* point2{};

		for (TActorIterator<AAIEnemyTargetPoint> It(GetWorld()); It; ++It) {

			UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), It->GetActorLocation(), PlayerCharacter);

			if (!path->IsPartial()) {
				if (path->GetPathLength() < dist_min) {
					dist_min = path->GetPathLength();
					point1 = *It;
				}
			}

		}

		dist_min = INFINITY;

		for (TActorIterator<AAIEnemyTargetPoint> It(GetWorld()); It; ++It) {

			if (point1->Position != It->Position) {
				UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), It->GetActorLocation(), PlayerCharacter);

				if (!path->IsPartial()) {
					if (path->GetPathLength() < dist_min) {
						dist_min = path->GetPathLength();
						point2 = *It;
					}
				}
			}

		}

		// DEBUG LINE
		DrawDebugLine(GetWorld(), point1->GetActorLocation() + FVector{ 0.0f,0.0f,20.0f }, point2->GetActorLocation() + FVector{ 0.0f,0.0f,20.0f }, FColor{ 255,0,0 }, false, 5.f, (uint8)'\000', 10.f);

		UNavigationPath* path1 = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), point1->GetActorLocation(), PlayerCharacter);
		UNavigationPath* path2 = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), point2->GetActorLocation(), PlayerCharacter);

		AAIEnemyTargetPoint* pointChoisi;

		if (path1->GetPathLength() < path2->GetPathLength()) {
			pointChoisi = point2;
		}
		else {
			pointChoisi = point1;
		}

		BlackboardComponent->SetValueAsInt("TargetPointNumber", pointChoisi->Position);
		BlackboardComponent->SetValueAsVector("TargetPointPosition", pointChoisi->GetActorLocation());

		return EPathFollowingRequestResult::RequestSuccessful;

	}
}

TArray<APlayerState*> AAIEnemyController::GetClosestCharacters() {

	APawn* PawnUsed = GetPawn();

	AGameStateBase* GameState = GetWorld()->GetGameState<ALabyrinthGameStateBase>();
	TArray<APlayerState*> PlayerArray = GameState->PlayerArray;

	TArray<APlayerState*> ArrayFiltered = PlayerArray.FilterByPredicate([&](APlayerState* Player) {

		if (Player->IsABot())
			return false;

		APawn* PlayerPawn = Player->GetPawn();
		FVector PositionAI = PawnUsed->GetActorLocation();
		FVector PositionPlayer = PlayerPawn->GetActorLocation();

		FVector DistanceBetween = PositionPlayer - PositionAI;
		DistanceBetween.FVector::Normalize();

		FVector FVectorAI = PawnUsed->GetActorForwardVector();

		float DotProd = FVector::DotProduct(DistanceBetween, FVectorAI);

		if (UKismetMathLibrary::DegAcos(DotProd) < 90.0f)
		{
			AActor* AIActor = GetOwner();
			UCapsuleComponent* capsule = Cast<ACharacter>(PawnUsed)->GetCapsuleComponent();
			FCollisionQueryParams TraceParams(FName(TEXT("TraceAI2Player")), false, capsule->GetOwner());
			//TraceParams.bReturnPhysicalMaterial = false;
			//TraceParams.bTraceComplex = false;

			FHitResult Hit(ForceInit);
			GetWorld()->LineTraceSingleByChannel(Hit, PositionAI, PositionPlayer, ECC_Camera, TraceParams);
			ALabCharacter* CastedActor = Cast<ALabCharacter>(Hit.GetActor());

			if (!CastedActor) {
				return false;
			}
			else {
				return true;
			}
		}

		else
			return false;

		});

	/*auto new_end = std::remove_if(PlayerArray.begin(), PlayerArray.end(), [&](APlayerState& Player) {

		if (Player.IsABot())
			return true;
		
		APawn* PlayerPawn = Player.GetPawn();
		FVector PositionAI = PawnUsed->GetActorLocation();
		FVector PositionPlayer = PlayerPawn->GetActorLocation();

		FVector DistanceBetween = PositionPlayer - PositionAI;
		DistanceBetween.FVector::Normalize();

		FVector FVectorAI = PawnUsed->GetActorForwardVector();

		float DotProd = FVector::DotProduct(DistanceBetween, FVectorAI);

		if (UKismetMathLibrary::DegAcos(DotProd) < 90.0f)
		{
			AActor* AIActor = GetOwner();
			UCapsuleComponent* capsule = Cast<ACharacter>(PawnUsed)->GetCapsuleComponent();
			FCollisionQueryParams TraceParams(FName(TEXT("TraceAI2Player")), false, capsule->GetOwner());
			//TraceParams.bReturnPhysicalMaterial = false;
			//TraceParams.bTraceComplex = false;

			FHitResult Hit(ForceInit);
			GetWorld()->LineTraceSingleByChannel(Hit, PositionAI, PositionPlayer, ECC_Camera, TraceParams);
			ALabCharacter* CastedActor = Cast<ALabCharacter>(Hit.GetActor());

			return !CastedActor;
		}

		else
			return true;

	});*/

	ArrayFiltered.Sort([&](const APlayerState& PlayerState1, const APlayerState& PlayerState2) {

		APawn* PlayerPawn1 = PlayerState1.GetPawn();
		FVector PositionAI = PawnUsed->GetActorLocation();
		FVector PositionPlayer1 = PlayerPawn1->GetActorLocation();

		APawn* PlayerPawn2 = PlayerState2.GetPawn();
		FVector PositionPlayer2 = PlayerPawn2->GetActorLocation();

		float distance1 = FVector::Distance(PositionAI, PositionPlayer1);

		float distance2 = FVector::Distance(PositionAI, PositionPlayer2);

		return distance1 < distance2;

	});

	/*std::sort(PlayerArray.begin(), new_end, [&](APlayerState* PlayerState1, APlayerState* PlayerState2) {

		APawn* PlayerPawn1 = PlayerState1->GetPawn();
		FVector PositionAI = PawnUsed->GetActorLocation();
		FVector PositionPlayer1 = PlayerPawn1->GetActorLocation();

		APawn* PlayerPawn2 = PlayerState2->GetPawn();
		FVector PositionPlayer2 = PlayerPawn2->GetActorLocation();

		float distance1 = FVector::Distance(PositionAI, PositionPlayer1);

		float distance2 = FVector::Distance(PositionAI, PositionPlayer2);

		return distance1 < distance2;

	});*/

	return ArrayFiltered;

}