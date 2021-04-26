#pragma once

#include "Core.h"
#include "Room.h"
#include "PuzzleRoom.generated.h"

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

public:

	APuzzleRoom();

	virtual void BeginPlay() override;

	virtual void InitPuzzle(FRandomStream seed);


	AActor* InstanceBP(const TCHAR* bpName, FVector location, FRotator rotation, FVector scale);
};
