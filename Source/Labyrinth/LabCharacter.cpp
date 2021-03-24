// Fill out your copyright notice in the Description page of Project Settings.


#include "LabCharacter.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "UsableActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "LabyrinthPlayerController.h"
#include "LabyrinthGameInstance.h"


// Sets default values
ALabCharacter::ALabCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Vitesse = 0.5f;
	MaxUseDistance = 800;

	UCapsuleComponent* capsule = GetCapsuleComponent();
	capsule->SetNotifyRigidBodyCollision(true);
	GetMesh()->SetNotifyRigidBodyCollision(true);

}

// Called when the game starts or when spawned
void ALabCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine)
	{
		if(HasAuthority())
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Voici FPSCharacter!"));
	}
}

// Called every frame
void ALabCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Controller && Controller->IsLocalController())
	{
		AUsableActor* Usable = GetUsableInView();
		// Terminer le focus sur l'objet pr�c�dent
		if (FocusedUsableActor != Usable)
		{
			if (FocusedUsableActor)
			{
				FocusedUsableActor->OnEndFocus();
			}
			bHasNewFocus = true;
		}
		// Assigner le nouveau focus (peut �tre nul )
		FocusedUsableActor = Usable;
		// D�marrer un nouveau focus si Usable != null;
		if (Usable)
		{
			if (bHasNewFocus)
			{
				Usable->OnBeginFocus();
				bHasNewFocus = false;
			}
		}
	}
}

// Called to bind functionality to input
void ALabCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// On associe des assignations du gameplay � des traitements
	PlayerInputComponent->BindAxis("Forward", this, &ALabCharacter::Forward);
	PlayerInputComponent->BindAxis("Right", this, &ALabCharacter::Right);
	PlayerInputComponent->BindAxis("LookRight", this, &ALabCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ALabCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALabCharacter::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ALabCharacter::OnStopJump);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ALabCharacter::OnStartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ALabCharacter::OnStopRun);

	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ALabCharacter::Use);
	PlayerInputComponent->BindAction("Spray", IE_Pressed, this, &ALabCharacter::ShowSelectionWheel);
	PlayerInputComponent->BindAction("Spray", IE_Released, this, &ALabCharacter::UnShowSelectionWheel);
	PlayerInputComponent->BindAction("Click", IE_Released, this, &ALabCharacter::Draw);

}

void ALabCharacter::Forward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// Trouver o� est l'avant
		FRotator Rotation = Controller->GetControlRotation();
		// Ne pas tenir compte du pitch
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// Ajouter le mouvement dans la direction Avant � construire le vecteur
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value * Vitesse);
	}

}

void ALabCharacter::Right(float Value)
{

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// Trouver o� est la droite
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// Ajouter le mouvement dans cette direction
		AddMovementInput(Direction, Value * Vitesse);
	}

}

void ALabCharacter::OnStartJump()
{
	bPressedJump = true;
}

void ALabCharacter::OnStopJump()
{
	bPressedJump = false;
}

bool ALabCharacter::IsRunning()
{
	return bPressedRun;
}



void ALabCharacter::Use()
{
	if (HasAuthority())
	{
		AUsableActor* Usable = GetUsableInView();
		if (Usable)
		{
			ClientUse(Usable);
		}
	}
	else
	{
		ServerUse();
	}
}

void ALabCharacter::ShowSelectionWheel()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Selection wheel shown"));
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());

	if (IsValid(playerController) && playerController->IsLocalController() && playerController->bHasChalk && IsValid(playerController->SelectionWheel))
	{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Selection wheel shown"));
			playerController->SetIgnoreLookInput(true);
			playerController->bShowMouseCursor = true;
			playerController->SelectionWheel->AddToViewport();
			playerController->SetInputMode(FInputModeGameAndUI());
	}
}

void ALabCharacter::UnShowSelectionWheel()
{
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());

	if (IsValid(playerController) && playerController->IsLocalController() && playerController->bHasChalk && IsValid(playerController->SelectionWheel))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Removed Selection wheel"));
		playerController->SetIgnoreLookInput(false);
		playerController->bShowMouseCursor = false;
		float Angle = Cast<USelectionWheelUserWidget>(playerController->SelectionWheel)->GetAngle();

		if (Angle >= -180 && Angle <= 180)
		{
			TypeDraw sprayType = TypeDraw::FRONT_ARROW;

			if (Angle < 30.f && Angle > -30.f)
				sprayType = TypeDraw::QUESTION_MARK;
			else if (Angle > 30.f && Angle < 90.f)
				sprayType = TypeDraw::CIRCLE;
			else if (Angle > 90.f && Angle < 150.f)
				sprayType = TypeDraw::RIGHT_ARROW;
			else if (Angle > 150.f && Angle < -150.f)
				sprayType = TypeDraw::FRONT_ARROW;
			else if (Angle < -90.f && Angle > -150.f)
				sprayType = TypeDraw::LEFT_ARROW;
			else if (Angle < -30.f && Angle > -90.f)
				sprayType = TypeDraw::CROSS;

			FTransform transf = GetPositionInView();
			FVector pos = transf.GetLocation();

			// Limit of chalk : how far can the center of the spray be set?
			// Also making sure that we're not spraying the void.
			if (!(FVector::Distance(transf.GetLocation(), GetActorLocation()) >= 250.f || FVector::Distance(pos, FVector{ 0, 0, 0 }) <= 1e-1))
			{
				FVector normale = transf.GetLocation() - GetActorLocation();
				FRotator sprayRotation = UKismetMathLibrary::MakeRotationFromAxes(UKismetMathLibrary::GetVectorArrayAverage(TArray<FVector>{transf.GetScale3D(), -normale}), -GetActorRightVector(), GetActorUpVector());
				DrawDebugLine(GetWorld(), GetActorLocation(), pos, FColor::Blue, true);

				ServerSpray(sprayType, pos, sprayRotation);
			}
		}

		playerController->SelectionWheel->RemoveFromViewport();
		playerController->SetInputMode(FInputModeGameOnly());
	}
	
}

void ALabCharacter::Draw()
{
	if (SelectionWheel) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Drew something"));
		float Angle = Cast<USelectionWheelUserWidget>(SelectionWheel)->GetAngle();
		ServerSpray(Angle);
		UnShowSelectionWheel();
	}
}

AActor* ALabCharacter::InstanceBP(const TCHAR* bpName, FVector location, FRotator rotation, FVector scale)
{
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, bpName));

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
			rotation,
			location,
			scale }, SpawnParams);
}
//
bool ALabCharacter::ServerSpray_Validate(TypeDraw sprayType, FVector pos, FRotator sprayRotation) {
	return true;
}

void ALabCharacter::ServerSpray_Implementation(TypeDraw sprayType, FVector pos, FRotator sprayRotation) {

	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());
	if (IsValid(playerController))
	{

		float sizeScale = 40.f;

		AActor* actor = InstanceBP(TEXT("/Game/Blueprints/Spray_BP.Spray_BP")
			, pos, sprayRotation);
		actor->SetActorScale3D({ sizeScale,sizeScale,sizeScale });

		AChalkDrawDecalActor* decal = Cast<AChalkDrawDecalActor>(actor);

		decal->kind = sprayType;
		
	}
	
}

void ALabCharacter::ServerUse_Implementation()
{
	Use();
}
bool ALabCharacter::ServerUse_Validate()
{
	return true;
}

void ALabCharacter::ClientUse_Implementation(AUsableActor* Usable)
{
	Usable->OnUsed(this);
}

AUsableActor* ALabCharacter::GetUsableInView()
{
	FVector CamLoc;
	FRotator CamRot;
	if (Controller == NULL)
		return NULL;
	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);
	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
	return Cast<AUsableActor>(Hit.GetActor());
}

FTransform ALabCharacter::GetPositionInView()
{
	FVector CamLoc;
	FRotator CamRot;
	if (Controller == NULL)
		return { FQuat{}, FVector{}, FVector{} }; // A CHANGER <----------------------------------------------------------------
	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);
	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
	return { FQuat{}, Hit.Location, Hit.Normal  };
}

void ALabCharacter::OnStartRun()
{
	Vitesse = 1.2f;
	bPressedRun = true;
}

void ALabCharacter::OnStopRun()
{
	Vitesse = 0.5f;
	bPressedRun = false;
}

void ALabCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{ 
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 
	DOREPLIFETIME(ALabCharacter, bNotSeenYet);
	DOREPLIFETIME(ALabCharacter, bHasNewFocus);
	DOREPLIFETIME(ALabCharacter, FocusedUsableActor);
}

