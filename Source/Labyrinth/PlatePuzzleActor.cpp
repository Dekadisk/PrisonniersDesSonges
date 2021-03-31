#include "PlatePuzzleActor.h"

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

void APlatePuzzleActor::OnUsed(AActor* InstigatorActor)
{
	Animate();
	ProcessTargetActions(bIsPressed);
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
		float currWeight = 0;
		
		for (AActor* actor : overlappingActors) {
			TArray<UPrimitiveComponent*> comps;
			actor->GetComponents(comps);
				
			for (UPrimitiveComponent* comp : comps) {
				if (comp)
					currWeight += comp->GetMass();
			}
		}

		if (currWeight > minWeight) {
			bIsPressed = true;

			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Press"));

			OnUsed(nullptr);
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
		float currWeight = 0;
		
		for (AActor* actor : overlappingActors) {
			TArray<UPrimitiveComponent*> comps;
			actor->GetComponents(comps);

			for (UPrimitiveComponent* comp : comps) {
				if (comp)
					currWeight += comp->GetMass();
			}
		}

		if (currWeight < minWeight) {
			bIsPressed = false;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Unpress"));

			OnUsed(nullptr);
		}
	}
}
