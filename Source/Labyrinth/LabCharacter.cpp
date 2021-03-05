// Fill out your copyright notice in the Description page of Project Settings.


#include "LabCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "UsableActor.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuUserWidget.h"


// Sets default values
ALabCharacter::ALabCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Vitesse = 0.5f;
	MaxUseDistance = 800;
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

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());

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
				// Pour d�bogage, vous pourrez l'oter par la suite
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Focus"));
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

