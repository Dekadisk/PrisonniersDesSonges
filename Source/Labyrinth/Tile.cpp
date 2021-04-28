#include "Tile.h"
#include "LabBlock.h"
#include "InfluencerActor.h"

// Sets default values
ATile::ATile()
{
	//bReplicates = true;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	inf_overlap = CreateDefaultSubobject<UBoxComponent>(TEXT("inf_overlap"));
	inf_overlap->InitBoxExtent({ LabBlock::assetSize/2, LabBlock::assetSize/2, LabBlock::assetSize/2 });
	inf_overlap->AttachToComponent(mesh,FAttachmentTransformRules::KeepRelativeTransform);

	SetRootComponent(mesh);
	kind = 0;

    // Mesh Caves
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshC1(TEXT("StaticMesh'/Game/Assets/Cave/Caves_C1.Caves_C1'"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshC2Corner(TEXT("StaticMesh'/Game/Assets/Cave/Caves_C2Corner.Caves_C2Corner'"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshC2Forward(TEXT("StaticMesh'/Game/Assets/Cave/Caves_C2Forward.Caves_C2Forward'"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshC3(TEXT("StaticMesh'/Game/Assets/Cave/Caves_C3.Caves_C3'"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshC4(TEXT("StaticMesh'/Game/Assets/Cave/Caves_C4.Caves_C4'"));

    // check if path is valid
	if (MeshC1.Succeeded())
		C1 = MeshC1.Object;
	if (MeshC2Corner.Succeeded())
		C2Corner = MeshC2Corner.Object;
	if (MeshC2Forward.Succeeded())
		C2Forward = MeshC2Forward.Object;
	if (MeshC3.Succeeded())
		C3 = MeshC3.Object;
	if (MeshC4.Succeeded())
		C4 = MeshC4.Object;

	//InfluenceMap
	for (int i = 0; i < 7; ++i)
		inf_values.Emplace(InfluenceGroup(i), 0.f);
}

void ATile::UpdateMesh()
{
	switch (kind) {

	case 0:
		Destroy();
		break;
	case 1:
		mesh->SetStaticMesh(C1);
		break;
	case 2:
		mesh->SetStaticMesh(C1);
		mesh->SetRelativeRotation(FQuat({0,0,1},PI/2));
		break;
	case 3:
		mesh->SetStaticMesh(C2Corner);
		mesh->SetRelativeRotation(FQuat({ 0,0,1 }, PI / 2));
		break;
	case 4:
		mesh->SetStaticMesh(C1);
		mesh->SetRelativeRotation(FQuat({ 0,0,1 }, PI));
		break;
	case 5:
		mesh->SetStaticMesh(C2Forward);
		break;
	case 6:
		mesh->SetStaticMesh(C2Corner);
		mesh->SetRelativeRotation(FQuat({ 0,0,1 }, PI));
		break;
	case 7:
		mesh->SetStaticMesh(C3);
		mesh->SetRelativeRotation(FQuat({ 0,0,1 }, -PI / 2));
		break;
	case 8:
		mesh->SetStaticMesh(C1);
		mesh->SetRelativeRotation(FQuat({ 0,0,1 }, -PI / 2));
		break;
	case 9:
		mesh->SetStaticMesh(C2Corner);
		break;
	case 10:
		mesh->SetStaticMesh(C2Forward);
		mesh->SetRelativeRotation(FQuat({ 0,0,1 }, PI / 2));
		break;
	case 11:
		mesh->SetStaticMesh(C3);
		mesh->SetRelativeRotation(FQuat({ 0,0,1 }, PI ));
		break;
	case 12:
		mesh->SetStaticMesh(C2Corner);
		mesh->SetRelativeRotation(FQuat({ 0,0,1 }, -PI/2));
		break;
	case 13:
		mesh->SetStaticMesh(C3);
		mesh->SetRelativeRotation(FQuat({ 0,0,1 }, PI/2));
		break;
	case 14:
		mesh->SetStaticMesh(C3);
		break;
	case 15:
		mesh->SetStaticMesh(C4);
		break;
	default:
		Destroy();
		break;

	}
}

void ATile::OnRep_UpdateMesh()
{
	UpdateMesh();
}

void ATile::UpdateInfluenceSources()
{
	TArray<AActor*> overlappingActors;
	inf_overlap->GetOverlappingActors(overlappingActors);
	for (AActor* actor : overlappingActors) {
		AInfluencerActor* influencer = Cast<AInfluencerActor>(actor);
		if (influencer && influencer->InfluenceDataAsset) {
			float inf_value = influencer->shouldUseAlternativeInfluence() ? influencer->InfluenceDataAsset->alternativeInfluence : influencer->InfluenceDataAsset->influence;
			switch (influencer->InfluenceDataAsset->blendMode)
			{
			case BlendModes::Additive:
				inf_values.Emplace(influencer->InfluenceDataAsset->influenceGroup,
					inf_values[influencer->InfluenceDataAsset->influenceGroup] + inf_value);
				break;
			case BlendModes::AlphaAdditive:
				if(inf_values[influencer->InfluenceDataAsset->influenceGroup] == 0.f)
					inf_values.Emplace(influencer->InfluenceDataAsset->influenceGroup,
						inf_values[influencer->InfluenceDataAsset->influenceGroup] + inf_value);
				else
					inf_values.Emplace(influencer->InfluenceDataAsset->influenceGroup,
						inf_values[influencer->InfluenceDataAsset->influenceGroup] + influencer->InfluenceDataAsset->blendAlpha * inf_value);
				break;
			default:
				break;
			}
		}
	}
}

/*void ATile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATile, kind);
}*/
