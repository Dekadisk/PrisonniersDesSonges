#include "TrapActor.h"
#include "TrapHeld.h"
#include "LabCharacter.h"
#include "PlayerCharacter.h"
#include "LabyrinthPlayerController.h"

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

		JawButton->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::BeginOverlap);
		JawButton->OnComponentEndOverlap.AddDynamic(this, &ATrapActor::OnOverlapEnd);

		//JawLeft->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::BeginOverlap);
		//JawLeft->OnComponentEndOverlap.AddDynamic(this, &ATrapActor::OnOverlapEnd);

		//JawRight->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::BeginOverlap);
		//JawRight->OnComponentEndOverlap.AddDynamic(this, &ATrapActor::OnOverlapEnd);

		//JawBar->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::BeginOverlap);
		//JawBar->OnComponentEndOverlap.AddDynamic(this, &ATrapActor::OnOverlapEnd);
}

void ATrapActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsOpen && OtherActor->HasAuthority())
	{
		if (OverlappedComponent == JawButton && OtherActor != this)
		{
			MulticastClose();
			bIsOpen = false;
			if (Cast<ALabCharacter>(OtherActor)) {
				Cast<ALabCharacter>(OtherActor)->Trap();
				trappedCharacter = Cast<ALabCharacter>(OtherActor);
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Piège fermé sur un joueur."));
		}
	}
}

void ATrapActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (trappedCharacter != nullptr && OtherActor->HasAuthority()) {
		if (OverlappedComp == JawButton && OtherActor != this) {
			MulticastOpen();
			bIsOpen = true;
			if (Cast<ALabCharacter>(OtherActor)) {
				Cast<ALabCharacter>(OtherActor)->Untrap();
				trappedCharacter = nullptr;
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Piège ouvert."));
		}
	}
}

AActor* ATrapActor::SpawnHeld_BP()
{
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Blueprints/TrapHeld_BP.TrapHeld_BP")));

	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	if (!SpawnActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		return nullptr;
	}

	UClass* SpawnClass = SpawnActor->StaticClass();
	if (SpawnClass == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
		return nullptr;
	}

	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return World->SpawnActor<AActor>(GeneratedBP->GeneratedClass,
		FTransform{
			FRotator{0, 0, 0},
			FVector{0, 0, 0},
			FVector{0.1f, 0.1f, 0.1f}
		},
		SpawnParams);
}

void ATrapActor::Use(bool Event, APawn* InstigatorPawn)
{
	ALabCharacter* player = Cast<ALabCharacter>(InstigatorPawn);
	if (IsValid(player))
	{
		// If the trap is open, close it.
		if (bIsOpen) {
			MulticastClose();
			bIsOpen = false;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Piège fermé par un joueur."));
		}
		// If it's closed :
		else {
			// Otherwise, there is someone in it. It will open slowly.
			if (trappedCharacter != nullptr) {
				MulticastSlowlyOpen();
			}
			// If it's because it spawned this way (or someone closed it manually), just get it.
			else {

				ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(player->GetController());
				if (IsValid(playerController) && !playerController->bHasTrap)
				{
					Super::Use(Event, InstigatorPawn);
					
					playerController->bHasTrap = true;
					FActorSpawnParameters SpawnInfo;
					SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					
					ATrapHeld* TrapHeld_BP = Cast<ATrapHeld>(SpawnHeld_BP());
					bool res = TrapHeld_BP->GetLanternMesh()->AttachToComponent(player->GetMesh(),
																				FAttachmentTransformRules::SnapToTargetIncludingScale,
																				FName("thigh_l_trap"));
				}
			}

		}
	}
}

void ATrapActor::MulticastOpen_Implementation() {
	Open();
}

void ATrapActor::MulticastClose_Implementation() {
	Close();
}

void ATrapActor::MulticastSlowlyOpen_Implementation() {
	SlowlyOpen();
}

void ATrapActor::OnBeginFocus()
{
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		JawRight->SetRenderCustomDepth(true);
		JawLeft->SetRenderCustomDepth(true);
		JawBar->SetRenderCustomDepth(true);
		JawButton->SetRenderCustomDepth(true);
		//PorteD->SetRenderCustomDepth(true);
	}
}

void ATrapActor::OnEndFocus()
{
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
		JawRight->SetRenderCustomDepth(false);
		JawLeft->SetRenderCustomDepth(false);
		JawBar->SetRenderCustomDepth(false);
		JawButton->SetRenderCustomDepth(false);
	}
}

void ATrapActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATrapActor, trappedCharacter);
	DOREPLIFETIME(ATrapActor, bIsOpen);
}