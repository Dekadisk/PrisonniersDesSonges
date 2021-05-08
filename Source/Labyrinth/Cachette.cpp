#include "Cachette.h"
#include "LabCharacter.h"
#include "MonsterCharacter.h"
#include "LabyrinthPlayerController.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"

ACachette::ACachette() {


	Hitboite = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hitboite_MESH"));
	Hitboite->SetupAttachment(MeshComp);

	PorteG = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PorteG_MESH"));
	PorteG->SetupAttachment(MeshComp);

	PorteD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PorteD_MESH"));
	PorteD->SetupAttachment(MeshComp);

	TiroirG = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TiroirG_MESH"));
	TiroirG->SetupAttachment(MeshComp);

	TiroirD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TiroirD_MESH"));
	TiroirD->SetupAttachment(MeshComp);

	bIsOpen = false;
	bIsProcessing = false;
	SetReplicates(true);

	Hitboite->SetGenerateOverlapEvents(true);
	Hitboite->OnComponentBeginOverlap.AddDynamic(this, &ACachette::BeginOverlap);
	Hitboite->OnComponentEndOverlap.AddDynamic(this, &ACachette::OnOverlapEnd);
}

void ACachette::BeginPlay() {
	Super::BeginPlay();

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
}

void ACachette::Tick(float DeltaSeconds) {
	/*FString affichage = "Il y a ";
	affichage += FString::SanitizeFloat(InCupboardPlayers.Num());
	affichage += " joueurs.";
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, affichage);*/
}

void ACachette::Use(bool Event, APawn* InstigatorPawn)
{
	ALabCharacter* MyPawn = Cast<ALabCharacter>(InstigatorPawn);
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(MyPawn->GetController());

	if (!bIsProcessing) {
		if (playerController->bIsHidden) {
			bIsProcessing = true;
			MulticastOpen();
			bIsOpen = true;
			for (int numPlayer = 0; numPlayer < InCupboardPlayers.Num(); numPlayer++) {
				Cast<ALabyrinthPlayerController>(InCupboardPlayers[numPlayer]->GetController())->bIsHidden = false;
				InCupboardPlayers[numPlayer]->ServerUnhide();
			}
		}
		else if (playerController->bIsInCupboard && bIsOpen) {
			bIsProcessing = true;
			MulticastClose();
			for (int numPlayer = 0; numPlayer < InCupboardPlayers.Num(); numPlayer++) {
				Cast<ALabyrinthPlayerController>(InCupboardPlayers[numPlayer]->GetController())->bIsHidden = true;
				InCupboardPlayers[numPlayer]->ServerHide();
			}
			bIsOpen = false;
		}
		else if (!(playerController->bIsInCupboard) && bIsOpen) {
			bIsProcessing = true;
			MulticastClose();
			for (int numPlayer = 0; numPlayer < InCupboardPlayers.Num(); numPlayer++) {
				Cast<ALabyrinthPlayerController>(InCupboardPlayers[numPlayer]->GetController())->bIsHidden = true;
				InCupboardPlayers[numPlayer]->ServerHide();
			}
			bIsOpen = false;
		}
		else {
			bIsProcessing = true;
			MulticastOpen();
			for (int numPlayer = 0; numPlayer < InCupboardPlayers.Num(); numPlayer++) {
				Cast<ALabyrinthPlayerController>(InCupboardPlayers[numPlayer]->GetController())->bIsHidden = false;
				InCupboardPlayers[numPlayer]->ServerUnhide();
			}
			bIsOpen = true;
		}
	}
}

void ACachette::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		if (OverlappedComponent == Hitboite && OtherActor->IsA(APlayerCharacter::StaticClass()))
		{
			APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
			if (player != nullptr) {
				Cast<ALabyrinthPlayerController>(player->GetController())->bIsInCupboard = true;
				InCupboardPlayers.Add(player);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Le joueur entre dans l'armoire."));
			}
		}
		else if (OverlappedComponent == Hitboite && OtherActor->IsA(AMonsterCharacter::StaticClass())) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("L'IA entre dans l'armoire."));
			LaunchIAOpen();
		}
	}
}

void ACachette::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (HasAuthority()) {
		if (OverlappedComp == Hitboite && OtherActor->IsA(APlayerCharacter::StaticClass()))
		{
			APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
			if (player != nullptr) {

				InCupboardPlayers.Remove(player);
				Cast<ALabyrinthPlayerController>(player->GetController())->bIsInCupboard = false;
				Cast<ALabyrinthPlayerController>(player->GetController())->bIsHidden = false;
				Cast<APlayerCharacter>(player)->ServerUnhide();
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Le joueur est sorti de l'armoire."));
			}
			else {
				for (int i = 0; i < InCupboardPlayers.Num(); i++) {
					auto playerController = Cast<ALabyrinthPlayerController>(InCupboardPlayers[i]->GetController());
					ALabCharacter* character = Cast<ALabCharacter>(playerController->AcknowledgedPawn);
					if (character == OtherActor) {
						InCupboardPlayers.RemoveAt(i);
						playerController->bIsInCupboard = false;
						playerController->bIsHidden = false;
						Cast<APlayerCharacter>(playerController->GetPawn())->ServerUnhide();
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Le joueur est sorti de l'armoire."));
						return;
					}
				}
			}
		}
		else if (OtherActor->IsA(AMonsterCharacter::StaticClass())) {
			GetWorld()->GetTimerManager().ClearTimer(timerHandle);
		}
	}
}

void ACachette::OnBeginFocus()
{
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		PorteG->SetRenderCustomDepth(true);
		PorteD->SetRenderCustomDepth(true);
	}
}

void ACachette::OnEndFocus()
{
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
		PorteG->SetRenderCustomDepth(false);
		PorteD->SetRenderCustomDepth(false);
	}
}

void ACachette::MulticastOpen_Implementation()
{
	AllOpen();
}

void ACachette::MulticastRemovePlayer_Implementation(APlayerCharacter* player)
{
	if (player != nullptr)
		InCupboardPlayers.Remove(player);
}

void ACachette::MulticastClose_Implementation()
{
	Close();
}

void ACachette::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Ensure the fuze timer is cleared by using the timer handle
	GetWorld()->GetTimerManager().ClearTimer(timerHandle);
}

void ACachette::LaunchIAOpen()
{
	GetWorld()->GetTimerManager().ClearTimer(timerHandle);
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ACachette::IAWait, 1, true);
}

void ACachette::IAWait() {
	timeIABreak++;
	if (timeIABreak == 5) {
		bIsProcessing = true;
		MulticastOpen();
		timeIABreak = 0;
		for (int numPlayer = 0; numPlayer < InCupboardPlayers.Num(); numPlayer++) {
			Cast<ALabyrinthPlayerController>(InCupboardPlayers[numPlayer]->GetController())->bIsHidden = false;
			InCupboardPlayers[numPlayer]->ServerUnhide();
		}
		bIsOpen = true;
	}
}

bool ACachette::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor) const {

	static const FName NAME_AILineOfSight = FName(TEXT("CachetteLineOfSight"));

	FVector compLocation = Hitboite->GetComponentLocation();

	FHitResult HitResult;
	const bool bHitBoite = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, compLocation
		, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic))
		, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

	if (bHitBoite == false || (HitResult.Actor.IsValid() && HitResult.Actor->IsOwnedBy(this))) {
		OutSeenLocation = compLocation;
		OutSightStrength = 1;

		return true;
	}

	OutSightStrength = 0;
	return false;
}