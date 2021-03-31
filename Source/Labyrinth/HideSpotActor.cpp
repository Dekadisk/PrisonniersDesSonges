#include "HideSpotActor.h"
#include "PlayerCharacter.h"

void AHideSpotActor::OnUsed(AActor* InstigatorPawn)
{
	APlayerCharacter* MyPawn = Cast<APlayerCharacter>(InstigatorPawn);
	if (MyPawn)
	{
		if (!hiddenActor) {
			Super::OnUsed(InstigatorPawn);
			MeshComp->UPrimitiveComponent::SetCollisionProfileName(FName("IgnoreOnlyPawn"), true);
			MyPawn->Controller->GetCharacter()->SetActorLocation(GetActorLocation());

			FVector fwdController = MyPawn->Controller->GetCharacter()->GetActorForwardVector();
			float angle = acos(FVector::DotProduct(fwdController, GetActorForwardVector()));

			FRotator rotation{ FQuat{FVector{0,0,1}, angle} };

			MyPawn->GetController()->SetControlRotation(GetActorRotation());

			hiddenActor = MyPawn;
			MyPawn->Controller->SetIgnoreMoveInput(true);
		}
		else
		{
			if (MyPawn == hiddenActor)
			{
				//faire sortir le joueur
			}
		}
	}
}