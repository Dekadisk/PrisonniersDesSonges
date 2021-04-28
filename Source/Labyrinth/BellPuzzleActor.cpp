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

	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave0(TEXT("/Game/Assets/Audio/Bell/bellSound0.bellSound0"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave1(TEXT("/Game/Assets/Audio/Bell/bellSound1.bellSound1"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave2(TEXT("/Game/Assets/Audio/Bell/bellSound2.bellSound2"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave3(TEXT("/Game/Assets/Audio/Bell/bellSound3.bellSound3"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave4(TEXT("/Game/Assets/Audio/Bell/bellSound4.bellSound4"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave5(TEXT("/Game/Assets/Audio/Bell/bellSound5.bellSound5"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave6(TEXT("/Game/Assets/Audio/Bell/bellSound6.bellSound6"));
	NoteSounds.Add(bellSoundWave0.Object);
	NoteSounds.Add(bellSoundWave1.Object);
	NoteSounds.Add(bellSoundWave2.Object);
	NoteSounds.Add(bellSoundWave3.Object);
	NoteSounds.Add(bellSoundWave4.Object);
	NoteSounds.Add(bellSoundWave5.Object);
	NoteSounds.Add(bellSoundWave6.Object);

}

void ABellPuzzleActor::Use(bool Event, APawn* InstigatorPawn)
{
	CheckEvents(EPuzzleEventCheck::On, InstigatorPawn);
	NetMulticastAnimate(InstigatorPawn);
}

void ABellPuzzleActor::NetMulticastAnimate_Implementation(APawn* InstigatorPawn)
{
	UGameplayStatics::PlaySoundAtLocation(this, NoteSounds[note], GetActorLocation(), 1.0F,1.0F);

	Shell->UPrimitiveComponent::AddImpulse(FVector::DotProduct(InstigatorPawn->GetActorForwardVector(), GetActorRightVector()) * 5000 * GetActorRightVector());
	//Pendulum->UPrimitiveComponent::AddImpulse(FVector::DotProduct(InstigatorPawn->GetActorForwardVector(), GetActorRightVector()) * 5000 * GetActorRightVector());
}

void ABellPuzzleActor::OnBeginFocus()
{
	Super::OnBeginFocus();

	if (!bDisableFocus)
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage�
		Shell->SetRenderCustomDepth(true);
}

void ABellPuzzleActor::OnEndFocus()
{
	Super::OnEndFocus();

	if (!bDisableFocus)
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage�
		Shell->SetRenderCustomDepth(false);
}

void ABellPuzzleActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdateScale();
}

void ABellPuzzleActor::UpdateScale() {
	TArray<UActorComponent*> components;
	GetComponents(components);
	float scale = note * 0.01F + 0.05F;
	SetActorScale3D({ scale,scale,scale });
	ArmL->SetRelativeScale3D({ 1 + (6 - note) * 0.5F,1,1 });
	ArmR->SetRelativeScale3D({ 1 + (6 - note) * 0.5F,1,1 });
	
	Shell->SetMassOverrideInKg(NAME_None, Shell->CalculateMass());
	Pendulum->SetMassOverrideInKg(NAME_None, Pendulum->CalculateMass());
}

void ABellPuzzleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABellPuzzleActor, note);
}