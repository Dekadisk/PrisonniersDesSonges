#pragma once

#include "Core.h"
#include "Room.h"
#include "MushroomDecorator.h"
#include "RockDecorator.h"
#include "LanternPickUpActor.h"
#include "Components/BoxComponent.h"
#include "PuzzleRoom.generated.h"

UENUM()
enum class PuzzleDifficulty {
	Easy,
	Medium,
	Hard
};

UCLASS()
class LABYRINTH_API APuzzleRoom : public ARoom
{
	GENERATED_BODY()

public:

	/*    CaveRoomStart     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* spawnRoom;

	/** DECORATIONS    */
	/**    Rock     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* Rock;

	/** Mite  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* Mite;

	/** Difficulty*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty");
	PuzzleDifficulty difficulty;

	UPROPERTY(Replicated, BlueprintReadWrite)
	int currentSection;

	UPROPERTY()
	TArray<AActor*> decorationsTagDynamic;
	UPROPERTY()
	TArray<bool> decorationsAreBegin;
	//
	//BLUEPRINTS
	UPROPERTY()
	TSubclassOf<AMushroomDecorator> Mushroom_BP;
	UPROPERTY()
	TSubclassOf<ARockDecorator> Rock_BP;
	UPROPERTY()
	TSubclassOf<AActor> ChalkOnChair_BP;
	UPROPERTY()
	TSubclassOf<ALanternPickUpActor> LanternPickUpActor_BP;
	UPROPERTY()
	TSubclassOf<AActor> FireTorch_BP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UBoxComponent* MusicOverlapBox;

public:

	APuzzleRoom();

	virtual void BeginPlay() override;

	virtual void InitPuzzle(FRandomStream seed, PuzzleDifficulty _difficulty);

	AActor* InstanceBP(TSubclassOf<UObject> blueprint, FVector location, FRotator rotation, FVector scale);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
