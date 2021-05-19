#include "TrapActor.h"
#include "LabCharacter.h"
#include "PlayerCharacter.h"
#include "MonsterCharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/Controller.h"
#include "AIEnemyController.h"
#include "LabyrinthPlayerController.h"
#include "AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <random>

ATrapActor::ATrapActor()
{
		JawRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JawRight_MESH"));
		JawLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JawLeft_MESH"));
		JawButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JawButton_MESH"));
		JawBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JawBar_MESH"));
		OverlapAI = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapAI_OVERLAP"));
		Hitbox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hitbox"));

		JawRight->SetupAttachment(MeshComp);
		JawLeft->SetupAttachment(MeshComp);
		JawButton->SetupAttachment(MeshComp);
		JawBar->SetupAttachment(MeshComp);
		OverlapAI->SetupAttachment(MeshComp);
		Hitbox->SetupAttachment(MeshComp);

		SetReplicates(true);

		JawButton->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::BeginOverlap);
		JawButton->OnComponentEndOverlap.AddDynamic(this, &ATrapActor::OnOverlapEnd);

		OverlapAI->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor::BeginOverlap);
		OverlapAI->OnComponentEndOverlap.AddDynamic(this, &ATrapActor::OnOverlapEnd);


		static ConstructorHelpers::FClassFinder<ATrapHeld> TrapHeld_BP_F(TEXT("/Game/Blueprints/TrapHeld_BP"));
		TrapHeld_BP = TrapHeld_BP_F.Class;
}

void ATrapActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComponent == OverlapAI && HasAuthority() && bIsOpen && Cast<AMonsterCharacter>(OtherActor)) {

		auto monster = Cast<AMonsterCharacter>(OtherActor);

		if (monster) {
			AAIEnemyController* monstrocontrolus = Cast<AAIEnemyController>(monster->GetController());
			std::random_device rd;
			std::mt19937 prng{ rd() };
			std::uniform_int_distribution<int> dice{ 0, 99 };

			float chancesToDestroy = (monstrocontrolus->DataAsset->Level * monstrocontrolus->DataAsset->ChancesToDestroyObstaclePerLevel + monstrocontrolus->DataAsset->ChancesToDestroyObstaclePerTrapped * monstrocontrolus->DataAsset->BeingTrapped);
			if (dice(prng) < chancesToDestroy) {
				monstrocontrolus->GetBrainComponent()->GetBlackboardComponent()->SetValueAsObject("ObstacleToDestroy", this);
			}
		}
			
			

	}

	else if (bIsOpen && OtherActor->HasAuthority())
	{
		if ((OverlappedComponent == JawButton || OverlappedComponent == Hitbox || OverlappedComponent == OverlapAI) && OtherActor != this && (OtherActor->IsA(APlayerCharacter::StaticClass()) || OtherActor->IsA(AMonsterCharacter::StaticClass())))
		{
				MulticastClose();
				bIsOpen = false;
				if (Cast<ALabCharacter>(OtherActor)) {

					trappedCharacter = Cast<ALabCharacter>(OtherActor);
					Cast<ALabCharacter>(OtherActor)->Trap();
					if (Cast<AMonsterCharacter>(OtherActor)) {
						AMonsterCharacter* monstre = Cast<AMonsterCharacter>(OtherActor);
						AAIController* controller = Cast<AAIController>(monstre->GetController());
						UBrainComponent* brain = controller->GetBrainComponent();
						
						FVector piedG = monstre->GetMesh()->GetSocketLocation("LeftFoot");
						FVector piedD = monstre->GetMesh()->GetSocketLocation("RightFoot");
						FVector distG{ piedG.X - GetActorLocation().X, piedG.Y - GetActorLocation().Y, 0.f };
						FVector distD{ piedD.X - GetActorLocation().X, piedD.Y - GetActorLocation().Y, 0.f };
						if (distG.Size() < distD.Size())
							monstre->MulticastTrapped(0, this);
						else
							monstre->MulticastTrapped(1, this);
						Cast<AAIEnemyController>(controller)->DataAsset->BeingTrapped++;
						controller->StopMovement();
						brain->StopLogic(FString("Pieged"));
					}
					else {
						APlayerCharacter* pc = Cast<APlayerCharacter>(OtherActor);
						if (pc) {
							FVector piedG = pc->GetMesh()->GetSocketLocation("LeftFoot");
							FVector piedD = pc->GetMesh()->GetSocketLocation("RightFoot");
							FVector distG{ piedG.X - GetActorLocation().X, piedG.Y - GetActorLocation().Y, 0.f };
							FVector distD{ piedD.X - GetActorLocation().X, piedD.Y - GetActorLocation().Y, 0.f };
							if (distG.Size() < distD.Size())
								pc->MulticastPlayAnim(TrapAnim, 0);
							else
								pc->MulticastPlayAnim(TrapAnim, 1);

							if (pc->GetController()->IsLocalPlayerController()) {
								Cast<ALabyrinthPlayerController>(pc->GetController())->SetSwitch(FName("Respiration_joueur"), FName("Rapide"));
								Cast<ALabyrinthPlayerController>(pc->GetController())->SetSwitch(FName("Heartbeat"), FName("Agite"));
								Cast<ALabyrinthPlayerController>(pc->GetController())->PlayMusic(pc->Respiration);
							}
						}
					}
				}
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Pi�ge ferm� sur un joueur."));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Pi�ge ferm� sur un joueur."));
		}
	}
}

void ATrapActor::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (trappedCharacter != nullptr && OtherActor->HasAuthority()) {
		if (OtherActor != this && OtherActor == trappedCharacter) {
			MulticastOpen();
			bIsOpen = true;
			if (Cast<ALabCharacter>(OtherActor)) {
				Cast<ALabCharacter>(OtherActor)->Untrap();
				trappedCharacter = nullptr;
			}
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Pi�ge ouvert."));
		}
	}
}

void ATrapActor::DestroyTrap()
{
	Destroy();
}

AActor* ATrapActor::SpawnHeld_BP()
{
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return World->SpawnActor<AActor>(TrapHeld_BP,
		FTransform{
			FRotator{0, 0, 0},
			FVector{0, 0, 0},
			FVector{0.1f, 0.1f, 0.1f}
		},
		SpawnParams);
}

void ATrapActor::Use(bool Event, APawn* InstigatorPawn)
{
	//Super::Use(Event, InstigatorPawn);
	ALabCharacter* player = Cast<ALabCharacter>(InstigatorPawn);
	if (IsValid(player))
	{
		// If the trap is open, close it.
		if (bIsOpen) {
			MulticastClose();
			bIsOpen = false;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Pi�ge ferm� par un joueur."));
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
					
					ATrapHeld* TrapHeld = Cast<ATrapHeld>(SpawnHeld_BP());
					bool res = TrapHeld->GetLanternMesh()->AttachToComponent(player->GetMesh(),
																				FAttachmentTransformRules::SnapToTargetIncludingScale,
																				FName("thigh_l_trap"));
					Cast<APlayerCharacter>(player)->trapHeld = TrapHeld;

				}
			}

		}
	}
}

void ATrapActor::Free_Implementation() {
	Cast<ALabCharacter>(trappedCharacter)->Untrap();
}

void ATrapActor::IAWait() {
	timeIABreak++;
	if (timeIABreak == 5) {
		MulticastOpen();
		timeIABreak = 0;
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);
		if (trappedCharacter && trappedCharacter->HasAuthority()) {
			AMonsterCharacter* monstre = Cast<AMonsterCharacter>(trappedCharacter);
			AAIController* controller = Cast<AAIController>(monstre->GetController());
			UBrainComponent* brain = controller->GetBrainComponent();
			brain->RestartLogic();
			Cast<ALabCharacter>(trappedCharacter)->Untrap();
			Destroy();
		}
		bIsOpen = true;
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
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage�
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
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage� 
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