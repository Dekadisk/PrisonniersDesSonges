// Fill out your copyright notice in the Description page of Project Settings.


#include "HideSpotActor.h"

#include "LabCharacter.h"

AHideSpotActor::AHideSpotActor() {

}

void AHideSpotActor::OnUsed(AActor* InstigatorPawn)
{


	ALabCharacter* MyPawn = Cast<ALabCharacter>(InstigatorPawn);
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