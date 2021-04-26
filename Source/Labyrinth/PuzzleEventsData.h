#pragma once

#include "GameFramework/Actor.h"
#include "Math/TransformNonVectorized.h"
#include "Engine/EngineTypes.h"
#include "PuzzleEventsData.generated.h"


class AUsableActor;

UENUM(BlueprintType)
enum class EPuzzleEventCheck : uint8 {
	None,
	On,
	Off,
	Lock,
	Unlock,
	Overlap,
	EndOverlap,
	AlternativeUse,
};

UENUM(BlueprintType)
enum class EPuzzleEventInteraction : uint8 {
	None,
	Use,
	AlternativeUse,
	On,
	Off,
	Lock,
	Unlock,
	Ring,
	Activate
};



// ENVIRONMENT

USTRUCT(BlueprintType)
struct FPE_ActorInteractions {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<AUsableActor*> Actors;

	UPROPERTY(EditAnywhere)
	EPuzzleEventInteraction Interaction;

	UPROPERTY(EditAnywhere)
	int RepeatNbTimes = 0;

	UPROPERTY(EditAnywhere)
	float DelayBefore = 0;

	UPROPERTY(EditAnywhere)
	float DelayBetweenRepeats = 0;
};

USTRUCT(BlueprintType)
struct FPE_ActorTransformationsSeq {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform TransformTo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Smooth = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0;
};

USTRUCT(BlueprintType)
struct FPE_ActorTransformations {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPE_ActorTransformationsSeq> Sequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Delay = 0;
};

USTRUCT(BlueprintType)
struct FPE_ActorSpawn {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> FromClass;

	UPROPERTY(EditAnywhere)
	FTransform SpawnTransform;

	UPROPERTY(EditAnywhere)
	ESpawnActorCollisionHandlingMethod CollisionHandling;

	UPROPERTY(EditAnywhere)
	TArray<FName> Tags;

	UPROPERTY(EditAnywhere)
	float Delay = 0;
};

USTRUCT(BlueprintType)
struct FPE_ActorHiding {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<AActor*> Actors;

	UPROPERTY(EditAnywhere)
	TArray<FName> WithTags;

	UPROPERTY(EditAnywhere)
	bool Hide = false;

	UPROPERTY(EditAnywhere)
	bool DisableCollision = false;

	UPROPERTY(EditAnywhere)
	bool Destroy = false;

	UPROPERTY(EditAnywhere)
	float Delay = 0;
};

USTRUCT(BlueprintType)
struct FPE_Environment {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FPE_ActorInteractions> ActorInteractions;
	UPROPERTY(EditAnywhere)
	TArray<FPE_ActorTransformations> ActorTransformations;
	UPROPERTY(EditAnywhere)
	TArray<FPE_ActorSpawn> ActorSpawns;
	UPROPERTY(EditAnywhere)
	TArray<FPE_ActorHiding> ActorHiding;
};



// PLAYER

USTRUCT(BlueprintType)
struct FPE_LookAtActorsSeq {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Smooth = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0;
};

USTRUCT(BlueprintType)
struct FPE_LookAtActors {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPE_LookAtActorsSeq> Sequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Delay = 0;
};

USTRUCT(BlueprintType)
struct FPE_Players {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FPE_LookAtActors> LookAt;
};



// SOUNDS

USTRUCT(BlueprintType)
struct FPE_2DSound {
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FPE_3DSound {
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FPE_AmbientSound {
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FPE_StopAmbientSound {
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FPE_Sounds {
	GENERATED_BODY()
};



// OBJECTIVE

USTRUCT(BlueprintType)
struct FPE_UpdateObjective {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Objective;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Delay = 0;
};

USTRUCT(BlueprintType)
struct FPE_Objectives {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPE_UpdateObjective> UpdateObjectives;
};



// SUBTITLES

USTRUCT(BlueprintType)
struct FPE_Subtitle {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Subtitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0;
};

USTRUCT(BlueprintType)
struct FPE_SubtitleSeq {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPE_Subtitle> Sequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Delay = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Broadcast;
};

USTRUCT(BlueprintType)
struct FPE_Subtitles {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPE_SubtitleSeq> Subtitles;
};



// PUZZLE EVENT

USTRUCT(BlueprintType)
struct FPE_PuzzleEvent {
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere)
	FPE_Environment Environment;

	UPROPERTY(EditAnywhere)
	FPE_Players Players;

	UPROPERTY(EditAnywhere)
	FPE_Sounds Sounds;

	UPROPERTY(EditAnywhere)
	FPE_Objectives Objectives;

	UPROPERTY(EditAnywhere)
	FPE_Subtitles Subtitles;
};




// MASTER EVENT
USTRUCT(BlueprintType)
struct FPE_PuzzleEventMaster {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	EPuzzleEventCheck Trigger;
	UPROPERTY(EditAnywhere)
	int AfterNbTimes = 1;
	UPROPERTY(VisibleAnywhere)
	int Counter = 0;
	UPROPERTY(EditAnywhere)
	bool OnlyOnce = true;
	UPROPERTY(EditAnywhere)
	FPE_PuzzleEvent Event;
};