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
		MeshComp->SetGenerateOverlapEvents(true);
		MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::BeginOverlap);
		MeshComp->OnComponentEndOverlap.AddDynamic(this, &ATrapActor::OnOverlapEnd);
}

void ATrapActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		if (OverlappedComponent == MeshComp)
		{
		}
	}
}

void ATrapActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (HasAuthority()) {
		if (OverlappedComp == MeshComp) {

		}
	}
}