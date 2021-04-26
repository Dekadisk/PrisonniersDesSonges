#include "BellHintActor.h"
#include "Components/AudioComponent.h"

ABellHintActor::ABellHintActor() {
	HintMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HintMesh"));

	HintMesh->SetupAttachment(MeshComp);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->OnAudioFinished.AddDynamic(this, &ABellHintActor::NextNote);

	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave0(TEXT("/Game/Assets/Audio/Bell/bellSound0.bellSound0"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave1(TEXT("/Game/Assets/Audio/Bell/bellSound1.bellSound1"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave2(TEXT("/Game/Assets/Audio/Bell/bellSound2.bellSound2"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave3(TEXT("/Game/Assets/Audio/Bell/bellSound3.bellSound3"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave4(TEXT("/Game/Assets/Audio/Bell/bellSound4.bellSound4"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave5(TEXT("/Game/Assets/Audio/Bell/bellSound5.bellSound5"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave6(TEXT("/Game/Assets/Audio/Bell/bellSound6.bellSound6"));
	NotesSamples.Add(bellSoundWave0.Object);
	NotesSamples.Add(bellSoundWave1.Object);
	NotesSamples.Add(bellSoundWave2.Object);
	NotesSamples.Add(bellSoundWave3.Object);
	NotesSamples.Add(bellSoundWave4.Object);
	NotesSamples.Add(bellSoundWave5.Object);
	NotesSamples.Add(bellSoundWave6.Object);
}

void ABellHintActor::NextNote()
{
	lastPlayed++;
	if (lastPlayed == NotesSounds.Num()) {
		lastPlayed = 0;
	}
	AudioComponent->SetSound(NotesSounds[lastPlayed]);
	if(lastPlayed != 0)
		AudioComponent->Play();
}

void ABellHintActor::NetMulticastAnimate_Implementation(APawn* InstigatorPawn)
{
	if (NotesSounds.Num() == 0)
		for (int32 w : waited)
		{
			NotesSounds.Add(NotesSamples[w]);
		}
	AudioComponent->SetSound(NotesSounds[0]);
	//Mettre le isProcessing dans le animate 
	isProcessing = true;
	Animate();
	AudioComponent->Play();
	isProcessing = false;
}

void ABellHintActor::Use(bool Event, APawn* InstigatorPawn)
{
	NetMulticastAnimate(InstigatorPawn);
	CheckEvents(EPuzzleEventCheck::On, InstigatorPawn);
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
