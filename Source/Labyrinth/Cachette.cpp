// Fill out your copyright notice in the Description page of Project Settings.


#include "Cachette.h"
#include "LabCharacter.h"
#include "LabyrinthPlayerController.h"

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

void ACachette::Tick(float DeltaSeconds) {
	FString affichage = "Il y a ";
	affichage += FString::SanitizeFloat(InCupboardPlayers.Num());
	affichage += " joueurs.";
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, affichage);
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
				InCupboardPlayers[numPlayer]->bIsHidden = false;
			}
		}
		else if (playerController->bIsInCupboard && bIsOpen) {
			bIsProcessing = true;
			MulticastClose();
			for (int numPlayer = 0; numPlayer < InCupboardPlayers.Num(); numPlayer++) {
				InCupboardPlayers[numPlayer]->bIsHidden = true;
			}
			bIsOpen = false;
		}
		else if (!(playerController->bIsInCupboard) && bIsOpen) {
			bIsProcessing = true;
			MulticastClose();
			for (int numPlayer = 0; numPlayer < InCupboardPlayers.Num(); numPlayer++) {
				InCupboardPlayers[numPlayer]->bIsHidden = true;
			}
			bIsOpen = false;
		}
		else {
			bIsProcessing = true;
			MulticastOpen();
			for (int numPlayer = 0; numPlayer < InCupboardPlayers.Num(); numPlayer++) {
				InCupboardPlayers[numPlayer]->bIsHidden = false;
			}
			bIsOpen = true;
		}
	}
}

void ACachette::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		if (OverlappedComponent == Hitboite && OtherActor)
		{
			ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(Cast<ALabCharacter>(OtherActor)->GetController());
			if (playerController != nullptr) {
				playerController->bIsInCupboard = true;
				InCupboardPlayers.Add(playerController);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Le joueur entre dans l'armoire."));
			}
		}
	}
}

void ACachette::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (HasAuthority()) {
		if (OverlappedComp == Hitboite && OtherActor)
		{
			ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(Cast<ALabCharacter>(OtherActor)->GetController());
			if (playerController != nullptr) {

				InCupboardPlayers.Remove(playerController);
				playerController->bIsInCupboard = false;
				playerController->bIsHidden = false;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Le joueur est sorti de l'armoire."));
			}
			else {
				for (int i = 0; i < InCupboardPlayers.Num(); i++) {
					ALabCharacter* character = Cast<ALabCharacter>(InCupboardPlayers[i]->AcknowledgedPawn);
					if (character == OtherActor) {
						playerController = InCupboardPlayers[i];
						InCupboardPlayers.Remove(playerController);
						playerController->bIsInCupboard = false;
						playerController->bIsHidden = false;
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Le joueur est sorti de l'armoire."));
						return;
					}
				}
			}
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

void ACachette::MulticastRemovePlayer_Implementation(ALabyrinthPlayerController* playerController)
{
	if (playerController != nullptr)
		InCupboardPlayers.Remove(playerController);
}

void ACachette::MulticastClose_Implementation()
{
	Close();
}
