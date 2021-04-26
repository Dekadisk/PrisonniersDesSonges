#include "TrapActor.h"
#include "LabCharacter.h"
#include "LabyrinthPlayerController.h"

ATrapActor::ATrapActor()
{
		JawRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JawRight_MESH"));
		JawLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JawLeft_MESH"));
		JawButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JawButton_MESH"));
		JawBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JawBar_MESH"));

		JawRight->SetupAttachment(MeshComp);
		JawLeft->SetupAttachment(MeshComp);
		JawButton->SetupAttachment(MeshComp);
		JawBar->SetupAttachment(MeshComp);

		SetReplicates(true);
		//JawButton->SetGenerateOverlapEvents(true);
		JawButton->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::BeginOverlap);
		JawButton->OnComponentEndOverlap.AddDynamic(this, &ATrapActor::OnOverlapEnd);

		JawLeft->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::BeginOverlap);
		JawLeft->OnComponentEndOverlap.AddDynamic(this, &ATrapActor::OnOverlapEnd);

		JawRight->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::BeginOverlap);
		JawRight->OnComponentEndOverlap.AddDynamic(this, &ATrapActor::OnOverlapEnd);

		JawBar->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::BeginOverlap);
		JawBar->OnComponentEndOverlap.AddDynamic(this, &ATrapActor::OnOverlapEnd);
}

void ATrapActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		if (OverlappedComponent == JawButton && OtherActor != this)
		{
			Close();
			bIsOpen = false;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Piège fermé sur un joueur."));
		}
	}
}

void ATrapActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (HasAuthority()) {
		if (OverlappedComp == JawButton && OtherActor != this) {
			Open();
			bIsOpen = true;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Piège ouvert."));
		}
	}
}

void ATrapActor::Use(bool Event, APawn* InstigatorPawn)
{
	ALabCharacter* player = Cast<ALabCharacter>(InstigatorPawn);
	if (IsValid(player))
	{
		// If the trap is open, close it.
		if (bIsOpen) {
			Close();
			bIsOpen = false;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Piège fermé par un joueur."));
		}
		// If it's closed, get it.
		else {
			ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(player->GetController());
			if (IsValid(playerController) && !playerController->bHasTrap)
			{
				Super::Use(Event, InstigatorPawn);
				playerController->bHasTrap = true;
			}
		}
	}
}

void ATrapActor::OnBeginFocus()
{
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		JawRight->SetRenderCustomDepth(true);
		JawLeft->SetRenderCustomDepth(true);
		JawBar->SetRenderCustomDepth(true);
		JawButton->SetRenderCustomDepth(true);
		//PorteD->SetRenderCustomDepth(true);
	}
}

void ATrapActor::OnEndFocus()
{
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
		JawRight->SetRenderCustomDepth(false);
		JawLeft->SetRenderCustomDepth(false);
		JawBar->SetRenderCustomDepth(false);
		JawButton->SetRenderCustomDepth(false);
		//PorteD->SetRenderCustomDepth(false);
	}
}