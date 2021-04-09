#include "BellPuzzleActor.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

ABellPuzzleActor::ABellPuzzleActor()
{
	Bell = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshBell"));
	BellStick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBellArm"));

	Bell->SetupAttachment(MeshComp);
	BellStick->SetupAttachment(MeshComp);

	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave(TEXT("/Game/Assets/Audio/Bell/bellSound.bellSound"));
	NoteSound = bellSoundWave.Object;

}

void ABellPuzzleActor::OnUsed(AActor* InstigatorActor)
{
	UGameplayStatics::PlaySoundAtLocation(this, NoteSound, GetActorLocation(), 1.0F, 1.0F/note);

	ProcessTargetActions(true);

	Bell->UPrimitiveComponent::AddImpulse(FVector::DotProduct(InstigatorActor->GetActorForwardVector(), GetActorRightVector()) * 5000 * GetActorRightVector(), FName("DEF_PENDULUM"), false);
	Bell->UPrimitiveComponent::AddImpulse(FVector::DotProduct(InstigatorActor->GetActorForwardVector(), GetActorRightVector()) * 6000 * GetActorRightVector(), FName("DEF_SHELL"), false);

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector::DotProduct(InstigatorActor->GetActorForwardVector(), GetActorForwardVector()) * 6000 * GetActorRightVector() * 100, FColor::Blue, true);
}

void ABellPuzzleActor::OnBeginFocus()
{
	Super::OnBeginFocus();

	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		Bell->SetRenderCustomDepth(true);
	}
}

void ABellPuzzleActor::BeginPlay() {
	Super::BeginPlay();
}

void ABellPuzzleActor::OnEndFocus()
{
	Super::OnEndFocus();

	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		Bell->SetRenderCustomDepth(false);
	}
}

void ABellPuzzleActor::OnConstruction(const FTransform& Transform)
{
	UpdateScale();
}

void ABellPuzzleActor::UpdateScale() {
	TArray<UActorComponent*> components;
	GetComponents(components);
	float scale = note * 0.10F + 1.0F;
	for (int32 numComp = 0; numComp < components.Num(); ++numComp)
	{
		USceneComponent* sc = Cast<USceneComponent>(components[numComp]);
		if (sc) {

			sc->SetRelativeScale3D({ scale,scale,scale });
		}
	}
}