#include "DoorActor.h"
#include "LabCharacter.h"
#include "LabyrinthPlayerController.h"

ADoorActor::ADoorActor() {
	bIsLocked = true;
	bIsOpen = false;
}

void ADoorActor::OnUsed(AActor* InstigatorActor)
{
	Super::OnUsed(InstigatorActor);
	ALabCharacter* LabCharacter = Cast<ALabCharacter>(InstigatorActor);
	if (LabCharacter && bIsLocked) 
	{
		ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(LabCharacter->GetController());
		if (IsValid(playerController))
		{
			if (playerController->bHasKey) {
				playerController->bHasKey = false;
				bIsLocked = false;
				bIsOpen = true;
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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Toggle door"));
	}
}

void ADoorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoorActor, bIsLocked);
	DOREPLIFETIME(ADoorActor, bIsOpen);
}
