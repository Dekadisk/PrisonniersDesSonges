#include "UsableActor.h"

AUsableActor::AUsableActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bDisableFocus = false;
	bReplicates = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void AUsableActor::OnUsed(AActor* InstigatorActor)
{
	// Rien ici, les classes dérivées s'en occuperont 
}

void AUsableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUsableActor, bDisableFocus);
}

void AUsableActor::OnBeginFocus()
{
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		MeshComp->SetRenderCustomDepth(true);
	}
}

void AUsableActor::OnEndFocus()
{
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
		MeshComp->SetRenderCustomDepth(false);
	}
}

