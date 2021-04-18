// Fill out your copyright notice in the Description page of Project Settings.


#include "EventMaker.h"
#include "UsableActor.h"
#include "BellPuzzleActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEventMaker::AEventMaker()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AEventMaker::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEventMaker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEventMaker::ExecuteEvent(FPE_PuzzleEvent pe)
{
	// DELAY FUNCTION <--------------------------------------------------------------------------------
	/*FTimerHandle UnusedHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("SpawnActors"), pe);
	GetWorldTimerManager().SetTimer(UnusedHandle, TimerDel, 5.0f, false);*/

	SpawnActors(pe);
	TransformActors(pe);
	InteractActors(pe);
	HideActors(pe);
	LookAtActors(pe);
	UpdateObjectives(pe);
	UpdateSubtitles(pe);
}

void AEventMaker::SpawnActors(FPE_PuzzleEvent pe)
{
	for (FPE_ActorSpawn e : pe.Environment.ActorSpawns) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Owner = GetOwner();
		SpawnInfo.Instigator = GetInstigator();
		AEventMaker* Maker = GetWorld()->SpawnActor<AEventMaker>(this->GetClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

		if (e.Delay > 0.0f) {
			FTimerHandle UnusedHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(Maker, FName("SpawnActor"), e);
			GetWorldTimerManager().SetTimer(UnusedHandle, TimerDel, e.Delay, false);
		}
		else {
			Maker->SpawnActor(e);
		}
	}
}

void AEventMaker::SpawnActor(FPE_ActorSpawn e)
{
	const FTransform SpawnTransform = e.SpawnTransform;
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = e.CollisionHandling;
	SpawnInfo.Owner = GetOwner();
	AActor* actor = GetWorld()->SpawnActor<AActor>(e.FromClass, SpawnTransform, SpawnInfo);
	actor->Tags = e.Tags;
}

void AEventMaker::TransformActors(FPE_PuzzleEvent pe)
{
	for (FPE_ActorTransformations e : pe.Environment.ActorTransformations) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Owner = GetOwner();
		SpawnInfo.Instigator = GetInstigator();
		AEventMaker* Maker = GetWorld()->SpawnActor<AEventMaker>(this->GetClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

		if (e.Delay > 0.0f) {
			FTimerHandle UnusedHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(Maker, FName("MulticastTransformActor"), e);
			GetWorldTimerManager().SetTimer(UnusedHandle, TimerDel, e.Delay, false);
		}
		else {
			Maker->MulticastTransformActor(e);
		}
	}
}

void AEventMaker::MulticastTransformActor_Implementation(FPE_ActorTransformations Event)
{
	TransformActor(Event);
}

void AEventMaker::InteractActors(FPE_PuzzleEvent pe)
{
	for (FPE_ActorInteractions e : pe.Environment.ActorInteractions) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Owner = GetOwner();
		SpawnInfo.Instigator = GetInstigator();
		AEventMaker* Maker = GetWorld()->SpawnActor<AEventMaker>(this->GetClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

		if (e.DelayBefore > 0.0f) {
			FTimerHandle UnusedHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(Maker, FName("InteractActor"), e);
			GetWorldTimerManager().SetTimer(UnusedHandle, TimerDel, e.DelayBefore, false);
		}
		else {
			Maker->InteractActor(e);
		}
	}
}
void AEventMaker::InteractActor(FPE_ActorInteractions e)
{
	for (AUsableActor* a : e.Actors)
	{
		switch (e.Interaction) {
		case EPuzzleEventInteraction::Use:
			a->Use(true, GetInstigator());
			break;
		case EPuzzleEventInteraction::AlternativeUse:
			a->AlternativeUse(true, GetInstigator());
			break;
		case EPuzzleEventInteraction::Unlock:
			a->Unlock(true);
			break;
		case EPuzzleEventInteraction::Lock:
			a->Lock(true);
			break;
		case EPuzzleEventInteraction::On:
			a->On(true);
			break;
		case EPuzzleEventInteraction::Off:
			a->Off(true);
			break;
		case EPuzzleEventInteraction::Ring:
			a->Ring(true, Cast<ABellPuzzleActor>(GetOwner())->note);
			break;
		case EPuzzleEventInteraction::Activate:
			a->Activate(true);
			break;
		}

		if (e.RepeatNbTimes > 0)
		{
			e.RepeatNbTimes--;
			FTimerHandle UnusedHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(this, FName("InteractActor"), e);
			GetWorldTimerManager().SetTimer(UnusedHandle, TimerDel, e.DelayBetweenRepeats, false);
		}
	}
}

void AEventMaker::HideActors(FPE_PuzzleEvent pe)
{
	for (FPE_ActorHiding e : pe.Environment.ActorHiding) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Owner = GetOwner();
		SpawnInfo.Instigator = GetInstigator();
		AEventMaker* Maker = GetWorld()->SpawnActor<AEventMaker>(this->GetClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

		if (e.Delay > 0.0f) {
			FTimerHandle UnusedHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(Maker, FName("HideActor"), e);
			GetWorldTimerManager().SetTimer(UnusedHandle, TimerDel, e.Delay, false);
		}
		else {
			Maker->HideActor(e);
		}
	}
}
void AEventMaker::HideActor(FPE_ActorHiding e)
{
	for (AActor* a : e.Actors) {
		if (e.Destroy) {
			a->Destroy();
		}
		else {
			a->SetActorHiddenInGame(e.Hide);
			MulticastDisableCollision(e.DisableCollision, a);
		}
	}

	for (FName tag : e.WithTags) {
		TArray<AActor*> tagged;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), tag, tagged);

		for (AActor* a : tagged) {
			if (e.Destroy) {
				a->Destroy();
			}
			else {
				a->SetHidden(e.Hide);
				MulticastDisableCollision(e.DisableCollision, a);
			}
		}
	}
}

void AEventMaker::MulticastDisableCollision_Implementation(bool disable, AActor* a)
{
	a->SetActorEnableCollision(!disable);
}

void AEventMaker::LookAtActors(FPE_PuzzleEvent pe)
{
	for (FPE_LookAtActors e : pe.Players.LookAt) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Owner = GetOwner();
		SpawnInfo.Instigator = GetInstigator();
		AEventMaker* Maker = GetWorld()->SpawnActor<AEventMaker>(this->GetClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

		if (e.Delay > 0.0f) {
			FTimerHandle UnusedHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(Maker, FName("MulticastLookAtActor"), e);
			GetWorldTimerManager().SetTimer(UnusedHandle, TimerDel, e.Delay, false);
		}
		else {
			Maker->MulticastLookAtActor(e);
		}
	}
}

void AEventMaker::MulticastLookAtActor_Implementation(FPE_LookAtActors Event)
{
	LookAtActor(Event);
}

void AEventMaker::UpdateObjectives(FPE_PuzzleEvent pe)
{
	for (FPE_UpdateObjective e : pe.Objectives.UpdateObjectives) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Owner = GetOwner();
		SpawnInfo.Instigator = GetInstigator();
		AEventMaker* Maker = GetWorld()->SpawnActor<AEventMaker>(this->GetClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

		if (e.Delay > 0.0f) {
			FTimerHandle UnusedHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(Maker, FName("MulticastUpdateObjective"), e);
			GetWorldTimerManager().SetTimer(UnusedHandle, TimerDel, e.Delay, false);
		}
		else {
			Maker->MulticastUpdateObjective(e);
		}
	}
}

void AEventMaker::MulticastUpdateObjective_Implementation(FPE_UpdateObjective Event)
{
	UpdateObjective(Event);
}

void AEventMaker::UpdateSubtitles(FPE_PuzzleEvent pe)
{
	for (FPE_SubtitleSeq e : pe.Subtitles.Subtitles) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Owner = GetOwner();
		SpawnInfo.Instigator = GetInstigator();
		AEventMaker* Maker = GetWorld()->SpawnActor<AEventMaker>(this->GetClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

		if (e.Delay > 0.0f) {
			FTimerHandle UnusedHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(Maker, FName("MulticastUpdateSubtitle"), e);
			GetWorldTimerManager().SetTimer(UnusedHandle, TimerDel, e.Delay, false);
		}
		else {
			Maker->MulticastUpdateSubtitle(e);
		}
	}
}

void AEventMaker::MulticastUpdateSubtitle_Implementation(FPE_SubtitleSeq Event)
{
	UpdateSubtitle(Event);
}

