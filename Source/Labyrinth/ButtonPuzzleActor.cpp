#include "ButtonPuzzleActor.h"
#include "PlayerCharacter.h"

// Called every frame
AButtonPuzzleActor::AButtonPuzzleActor()
{
	MeshButtonBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshButtonBase"));
	MeshButtonStick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshButtonStick"));

	MeshButtonBase->SetupAttachment(MeshComp);
	MeshButtonStick->SetupAttachment(MeshComp);
	isProcessing = false;
}

void AButtonPuzzleActor::OnUsed(AActor* InstigatorActor)
{
	if (!isProcessing)
	{
		Super::OnUsed(InstigatorActor);
		APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(InstigatorActor);
		if (MyCharacter)
		{
			ActivateAnimation();
			// Rajouter une fonction activate de l'objet qui sera lié au bouton
			if (HasAuthority())
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Activation du bouton a faire"));
		}
		
	}
}

void AButtonPuzzleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AButtonPuzzleActor, isProcessing);
}

void AButtonPuzzleActor::OnBeginFocus()
{
	Super::OnBeginFocus();

	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		MeshButtonBase->SetRenderCustomDepth(true);
		MeshButtonStick->SetRenderCustomDepth(true);
	}
}

void AButtonPuzzleActor::OnEndFocus()
{
	Super::OnEndFocus();
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		MeshButtonBase->SetRenderCustomDepth(false);
		MeshButtonStick->SetRenderCustomDepth(false);
	}
}