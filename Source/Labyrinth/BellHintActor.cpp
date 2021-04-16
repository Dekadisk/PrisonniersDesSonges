#include "BellHintActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

ABellHintActor::ABellHintActor() {
	HintMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HintMesh"));

	HintMesh->SetupAttachment(MeshComp);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->OnAudioFinished.AddDynamic(this, &ABellHintActor::NextNote);

	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave0(TEXT("/Game/Assets/Audio/Bell/bellSound.bellSound"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave1(TEXT("/Game/Assets/Audio/Bell/bellSound.bellSound"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave2(TEXT("/Game/Assets/Audio/Bell/bellSound.bellSound"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave3(TEXT("/Game/Assets/Audio/Bell/bellSound.bellSound"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave4(TEXT("/Game/Assets/Audio/Bell/bellSound.bellSound"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave5(TEXT("/Game/Assets/Audio/Bell/bellSound.bellSound"));
	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave6(TEXT("/Game/Assets/Audio/Bell/bellSound.bellSound"));
	NotesSamples.Add(bellSoundWave0.Object);
	NotesSamples.Add(bellSoundWave1.Object);
	NotesSamples.Add(bellSoundWave2.Object);
	NotesSamples.Add(bellSoundWave3.Object);
	NotesSamples.Add(bellSoundWave4.Object);
	NotesSamples.Add(bellSoundWave5.Object);
	NotesSamples.Add(bellSoundWave6.Object);

	NotesSamples[0]->Pitch = 1.f / (0 + 1);
	NotesSamples[1]->Pitch = 1.f / (1 + 1);
	NotesSamples[2]->Pitch = 1.f / (2 + 1);
	NotesSamples[3]->Pitch = 1.f / (3 + 1);
	NotesSamples[4]->Pitch = 1.f / (4 + 1);
	NotesSamples[5]->Pitch = 1.f / (5 + 1);
	NotesSamples[6]->Pitch = 1.f / (6 + 1);
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

void ABellHintActor::BeginPlay()
{
	Super::BeginPlay();
	//AudioComponent->SetSound(NotesSounds[0]);
}

void ABellHintActor::Use(bool Event, APawn* InstigatorPawn)
{
	if(NotesSounds.Num()==0)
	for (int32 w : waited)
	{
		NotesSounds.Add(NotesSamples[w]);
	}
	AudioComponent->SetSound(NotesSounds[0]);
	isProcessing = true;
	Animate();
	AudioComponent->Play();
	isProcessing = false;
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

void ABellHintActor::OnConstruction(const FTransform& Transform)
{
}
