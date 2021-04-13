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

	Hitboite->SetGenerateOverlapEvents(true);
	Hitboite->OnComponentBeginOverlap.AddDynamic(this, &ACachette::BeginOverlap);
	Hitboite->OnComponentEndOverlap.AddDynamic(this, &ACachette::OnOverlapEnd);
}

void ACachette::Use(bool Event, APawn* InstigatorPawn)
{
	ALabCharacter* MyPawn = Cast<ALabCharacter>(InstigatorPawn);
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(MyPawn->GetController());

	if (MyPawn->bIsHidden)
		AllOpen();
	else if (MyPawn->bIsInCupboard) {
		Close();
		MyPawn->bIsHidden = true;
	}
	else
		AllOpen();

	/*if (IsValid(MyPawn))
	{
		playerController = Cast<ALabyrinthPlayerController>(MyPawn->GetController());
		if (!MyPawn->bIsHidden) {
			Super::Use(Event, InstigatorPawn);
			MeshComp->UPrimitiveComponent::SetCollisionProfileName(FName("IgnoreOnlyPawn"), true);
			MyPawn->OnHide();
		}
		else {
			MyPawn->OnUnhide();
			MeshComp->UPrimitiveComponent::SetCollisionProfileName(FName("IgnoreOnlyPawn"), false);
			MeshComp->UPrimitiveComponent::SetCollisionProfileName(FName("Default"), true);
		}
	}*/
}

void ACachette::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ALabCharacter>(OtherActor)) 
	{
		Cast<ALabCharacter>(OtherActor)->bIsInCupboard = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Le joueur entre dans l'armoire."));
	}
}

void ACachette::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (Cast<ALabCharacter>(OtherActor)) 
	{
		Cast<ALabCharacter>(OtherActor)->bIsInCupboard = false;
		Cast<ALabCharacter>(OtherActor)->bIsHidden = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Le joueur est sorti dans l'armoire."));
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
		PorteD->SetRenderCustomDepth(true);
	}
}