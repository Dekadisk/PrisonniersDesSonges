#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "UsableActor.generated.h"

UCLASS()
class LABYRINTH_API AUsableActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AUsableActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Transient, Replicated)
	bool bDisableFocus;

public:	

	// Le joueur regarde l'objet
	virtual void OnBeginFocus();

	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus();

	// Appelé quand le joueur interagit avec l'objet
	virtual void OnUsed(AActor* InstigatorActor);

	//Multi
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
