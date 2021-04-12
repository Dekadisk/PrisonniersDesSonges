#include "LabCharacter.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "UsableActor.h"
#include "Kismet/GameplayStatics.h"
#include "LabyrinthPlayerController.h"
#include "LabyrinthGameInstance.h"
#include "MainMenuUserWidget.h"
#include "LobbyPlayerController.h"


// Sets default values
ALabCharacter::ALabCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Vitesse = 0.5f;
	MaxUseDistance = 500;

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

	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ALabCharacter::Use);
	PlayerInputComponent->BindAction("AlternativeUse", IE_Pressed, this, &ALabCharacter::AlternativeUse);

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

void ALabCharacter::Use()
{
	if (HasAuthority())
	{
		AUsableActor* Usable = GetUsableInView();
		if (Usable)
		{
			Usable->Use(false, this);
		}
	}
	else
	{
		ServerUse();
	}
}

void ALabCharacter::AlternativeUse()
{
	if (HasAuthority())
	{
		AUsableActor* Usable = GetUsableInView();
		if (Usable)
            ClientAlternativeUse(Usable);
	}
	else
	{
        ServerAlternativeUse();
	}
}
	

void ALabCharacter::Chat() {

	ALabyrinthPlayerController* pc = Cast<ALabyrinthPlayerController>(GetController());
	bool mouseShown = pc->bShowMouseCursor;
	pc->bShowMouseCursor = !mouseShown;
	if (mouseShown) {
		pc->ChatWidget->SetVisibility(ESlateVisibility::Hidden);
		pc->SetInputMode(FInputModeGameOnly());
	}
	else {
		pc->ChatWidget->SetVisibility(ESlateVisibility::Visible);
		pc->SetInputMode(FInputModeGameAndUI());
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

void ALabCharacter::ServerUse_Implementation()
{
	Use();
}

bool ALabCharacter::ServerUse_Validate()
{
	return true;
}


bool ALabCharacter::ServerAlternativeUse_Validate()
{
	return true;
}

void ALabCharacter::ServerAlternativeUse_Implementation()
{
	AlternativeUse();
}

void ALabCharacter::ClientUse_Implementation(AUsableActor* Usable)
{
	Usable->Use(false, this);
}

void ALabCharacter::ClientAlternativeUse_Implementation(AUsableActor* Usable)
{
	Usable->AlternativeUse(false, this);
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

FHitResult ALabCharacter::GetPositionInView()
{
	FVector CamLoc;
	FRotator CamRot;
	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);
	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
	return Hit;
}

void ALabCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{ 
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 
	DOREPLIFETIME(ALabCharacter, bNotSeenYet);
	DOREPLIFETIME(ALabCharacter, bHasNewFocus);
	DOREPLIFETIME(ALabCharacter, FocusedUsableActor);
}

