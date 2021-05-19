#include "BellHintActor.h"
#include "Components/AudioComponent.h"
#include "AkGameplayStatics.h"


ABellHintActor::ABellHintActor() {
	HintMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HintMesh"));

	HintMesh->SetupAttachment(MeshComp);
}

void ABellHintActor::NextNote()
{
	lastPlayed++;
	if (lastPlayed == WNotesSounds.Num()) {
		lastPlayed = 0;
		isProcessing = false;
	}
	if (lastPlayed != 0)
		UAkGameplayStatics::PostEvent(WNotesSounds[lastPlayed], this, 0, FOnAkPostEventCallback::FOnAkPostEventCallback());
}

void ABellHintActor::NetMulticastAnimate_Implementation(APawn* InstigatorPawn)
{
	if (WNotesSounds.Num() == 0)
		for (int32 w : waited)
		{
			WNotesSounds.Add(WNotesSamples[w]);
		}

	
	//Mettre le isProcessing dans le animate 
	isProcessing = true;
	UAkGameplayStatics::PostEvent(WNotesSounds[0], this, 0, FOnAkPostEventCallback::FOnAkPostEventCallback());
	
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABellHintActor::NextNote, 1.5f, false);

	
}

void ABellHintActor::Use(bool Event, APawn* InstigatorPawn)
{
	if (!isProcessing) {
		NetMulticastAnimate(InstigatorPawn);
		CheckEvents(EPuzzleEventCheck::On, InstigatorPawn);
	}
}

void ABellHintActor::OnBeginFocus()
{
	Super::OnBeginFocus();

	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		HintMesh->SetRenderCustomDepth(true);
	}
}

void ABellHintActor::OnEndFocus()
{
	Super::OnEndFocus();

	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		HintMesh->SetRenderCustomDepth(false);
	}
}

void ABellHintActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABellHintActor, waited);
}
