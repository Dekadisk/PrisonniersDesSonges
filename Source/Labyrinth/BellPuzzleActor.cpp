#include "BellPuzzleActor.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

ABellPuzzleActor::ABellPuzzleActor()
{
	Shell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shell_MESH"));
	Pendulum = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pendulum_MESH"));
	ArmL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmL_MESH"));
	ArmR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmR_MESH"));

	Shell->SetupAttachment(MeshComp);
	Pendulum->SetupAttachment(Shell);

	//LeftArm->SetupAttachment(Shell, TEXT("arm_l"));
	//RightArm->SetupAttachment(Shell, TEXT("arm_r"));

	ArmL->SetupAttachment(MeshComp, TEXT("arm_l"));
	ArmR->SetupAttachment(MeshComp, TEXT("arm_r"));

	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave(TEXT("/Game/Assets/Audio/Bell/bellSound.bellSound"));
	NoteSound = bellSoundWave.Object;

}

void ABellPuzzleActor::Use(bool Event, APawn* InstigatorPawn)
{
	CheckEvents(EPuzzleEventCheck::On, InstigatorPawn);
	UGameplayStatics::PlaySoundAtLocation(this, NoteSound, GetActorLocation(), 1.0F, 1.0F/note);

	Bell->UPrimitiveComponent::AddImpulse(InstigatorPawn->GetActorForwardVector() * 5000, FName("DEF_PENDULUM"), false);

	Shell->UPrimitiveComponent::AddImpulse(FVector::DotProduct(InstigatorActor->GetActorForwardVector(), GetActorRightVector()) * 5000 * GetActorRightVector());
	Pendulum->UPrimitiveComponent::AddImpulse(FVector::DotProduct(InstigatorActor->GetActorForwardVector(), GetActorRightVector()) * 6000 * GetActorRightVector());

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector::DotProduct(InstigatorActor->GetActorForwardVector(), GetActorForwardVector()) * 6000 * GetActorRightVector() * 100, FColor::Blue, true);
}

void ABellPuzzleActor::OnBeginFocus()
{
	Super::OnBeginFocus();

	if (!bDisableFocus)
	{
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage�
		Shell->SetRenderCustomDepth(true);
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
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage�
		Shell->SetRenderCustomDepth(false);
	}
}

void ABellPuzzleActor::OnConstruction(const FTransform& Transform)
{
	UpdateScale();
}

void ABellPuzzleActor::UpdateScale() {
	TArray<UActorComponent*> components;
	GetComponents(components);
	float scale = note * 0.10F + 0.1F;
	SetActorScale3D({ scale,scale,scale });
	ArmL->SetRelativeScale3D({ 1 + (6 - note) * 0.20F,1,1 });
	ArmR->SetRelativeScale3D({ 1 + (6 - note) * 0.20F,1,1 });
	/*for (int32 numComp = 0; numComp < components.Num(); ++numComp)
	{
		USceneComponent* sc = Cast<USceneComponent>(components[numComp]);
		if (sc) {

			sc->SetRelativeScale3D({ scale,scale,scale });
		}
	}*/
}