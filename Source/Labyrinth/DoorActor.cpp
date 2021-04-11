#include "DoorActor.h"
#include "LabCharacter.h"
#include "LabyrinthPlayerController.h"

ADoorActor::ADoorActor() {
	bIsLocked = true;
	bIsOpen = false;
}

void ADoorActor::Use(bool Event, APawn* InstigatorPawn)
{
	Super::Use(Event, InstigatorPawn);
	ALabCharacter* LabCharacter = Cast<ALabCharacter>(InstigatorPawn);
	if (LabCharacter && bIsLocked) 
	{
		ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(LabCharacter->GetController());
		if (IsValid(playerController))
		{
			if (playerController->bHasKey) {
				playerController->bHasKey = false;
				bIsLocked = false;
				bIsOpen = true;
				CheckEvents(EPuzzleEventCheck::Unlock, InstigatorPawn);
				CheckEvents(EPuzzleEventCheck::On, InstigatorPawn);
				OpenAnimation();
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cannot unlock door. (no key found)"));
			}
		}
		
	}
	else {
		bIsOpen = !bIsOpen;
		bIsOpen ? OpenAnimation() : CloseAnimation();
		bIsOpen ? CheckEvents(EPuzzleEventCheck::On, InstigatorPawn) : CheckEvents(EPuzzleEventCheck::Off, InstigatorPawn);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Toggle door"));
	}
}

void ADoorActor::OnRep_OpenCloseDoor()
{
	bIsOpen ? OpenAnimation() : CloseAnimation();
}

void ADoorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoorActor, bIsLocked);
	DOREPLIFETIME(ADoorActor, bIsOpen);
}
