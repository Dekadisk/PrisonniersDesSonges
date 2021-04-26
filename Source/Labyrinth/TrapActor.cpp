#include "TrapActor.h"

ATrapActor::ATrapActor()
{
		JawRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JawRight_MESH"));
		JawLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JawLeft_MESH"));
		JawButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JawButton_MESH"));
		JawBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JawBar_MESH"));

		JawRight->SetupAttachment(MeshComp);
		JawLeft->SetupAttachment(MeshComp);
		JawButton->SetupAttachment(MeshComp);
		JawBar->SetupAttachment(MeshComp);

		SetReplicates(true);
		JawButton->SetGenerateOverlapEvents(true);
		JawButton->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::BeginOverlap);
		JawButton->OnComponentEndOverlap.AddDynamic(this, &ATrapActor::OnOverlapEnd);
}

void ATrapActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		if (OverlappedComponent == JawButton)
		{
			Close();
		}
	}
}

void ATrapActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (HasAuthority()) {
		if (OverlappedComp == JawButton) {
			Open();
		}
	}
}