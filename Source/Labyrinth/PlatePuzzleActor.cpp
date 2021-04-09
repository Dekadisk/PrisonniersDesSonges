#include "PlatePuzzleActor.h"
#include <Labyrinth/PlayerCharacter.h>
#include <Labyrinth/MonsterCharacter.h>

APlatePuzzleActor::APlatePuzzleActor() {
	bDisableFocus = true;

	Plate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plate"));
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));

	Plate->AttachTo(MeshComp);
	OverlapBox->AttachTo(MeshComp);

	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &APlatePuzzleActor::OnOverlapBegin);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &APlatePuzzleActor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APlatePuzzleActor::BeginPlay()
{
	Super::BeginPlay();
	minWeight = minChar * 75.0f;
}

void APlatePuzzleActor::Use(bool Event, APawn* InstigatorPawn)
{
	Animate();
	bIsPressed ? CheckEvents(EPuzzleEventCheck::On, InstigatorPawn) : CheckEvents(EPuzzleEventCheck::Off, InstigatorPawn);
}

void APlatePuzzleActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap"));
	}
	
	if (!bIsPressed) {

		TArray<AActor*> overlappingActors;
		GetOverlappingActors(overlappingActors);
		int currChar = 0;
		
		ALabCharacter* InstigatorPawn = nullptr;
		for (AActor* actor : overlappingActors) {
			if (Cast<APlayerCharacter>(actor)) {
				currChar++;
				if (!InstigatorPawn)
					InstigatorPawn = Cast<APlayerCharacter>(actor);
			}
			else if (Cast<AMonsterCharacter>(actor))
				currChar += 2;
		}

		if (currChar > minChar) {
			bIsPressed = true;

			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Press"));

			Use(false, InstigatorPawn);
		}
	}

}

void APlatePuzzleActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("No more"));

	if (bIsPressed) {

		TArray<AActor*> overlappingActors;
		GetOverlappingActors(overlappingActors);
		int currChar = 0;

		for (AActor* actor : overlappingActors) {
			if (Cast<APlayerCharacter>(actor)) {
				currChar++;
			}
			else if (Cast<AMonsterCharacter>(actor))
				currChar += 2;
		}

		if (currChar < minChar) {
			bIsPressed = false;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Unpress"));

			Use(false, nullptr);
		}
	}
}
