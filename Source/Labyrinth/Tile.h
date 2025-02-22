#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "InfluenceDataAsset.h"
#include "Tile.generated.h"


UCLASS()
class LABYRINTH_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ATile();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshs");
	UStaticMeshComponent* mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite/*, ReplicatedUsing = OnRep_UpdateMesh*/);
	/// There are 16 kind of tiles :
	///  0 * * * | 1 *   * | 2 * * * | 3 *   *
	///    * * * |   *   * |   *     |   *
	///    * * * |   * * * |   * * * |   * * *
	/// 
	///  4 * * * | 5 *   * | 6 * * * | 7 *   *
	///    *   * |   *   * |   *     |   *
	///    *   * |   *   * |   *   * |   *   *
	/// 
	///  8 * * * | 9 *   * |10 * * * |11 *   *
	///        * |       * |         |        
	///    * * * |   * * * |   * * * |   * * *
	/// 
	/// 12 * * * |13 *   * |14 * * * |15 *   *
	///        * |       * |         |   
	///    *   * |   *   * |   *   * |   *   *
	int kind;

	/**    Cave C1     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* C1;

	/** Cave C2Corner  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* C2Corner;

	/** Cave C2Forward */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* C2Forward;

	/**     Cave C3    */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* C3;

	/**     Cave C4    */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* C4;

	UPROPERTY()
	int32 index=-1;

public:	

	UFUNCTION(BlueprintCallable)
	void UpdateMesh();

	UFUNCTION()
	void OnRep_UpdateMesh();

public:
	//InfluenceMap
	UPROPERTY(EditAnywhere)
	TMap<InfluenceGroup, float> inf_values;
	
	UPROPERTY(EditAnywhere)
	float inf_final;


	UPROPERTY(EditAnywhere)
	UBoxComponent* inf_overlap;

	UFUNCTION()
	void UpdateInfluenceSources();

	//Multi
	//void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

};
