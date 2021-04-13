#include "PlayerCharacter.h"
#include <Runtime/AIModule/Classes/Perception/AIPerceptionSystem.h>
#include <Runtime/AIModule/Classes/Perception/AISense_Sight.h>
#include "LabyrinthPlayerController.h"
#include "SelectionWheelUserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "LabyrinthGameModeBase.h"


APlayerCharacter::APlayerCharacter() :
	bRunning(false),
	bWaitFullRecovery(false),
	stamina(10),
	staminaMax(10)
{
	Vitesse = BaseSpeed;
}

void APlayerCharacter::BeginPlay() {
	Super::BeginPlay();

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Ensure the fuze timer is cleared by using the timer handle
	GetWorld()->GetTimerManager().ClearTimer(timerHandle);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// On associe des assignations du gameplay � des traitements
	PlayerInputComponent->BindAxis("Forward", this, &APlayerCharacter::Forward);
	PlayerInputComponent->BindAxis("Right", this, &APlayerCharacter::Right);
	PlayerInputComponent->BindAxis("LookRight", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::OnStartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::OnStopRun);

	PlayerInputComponent->BindAction("Spray", IE_Pressed, this, &APlayerCharacter::ShowSelectionWheel);
	PlayerInputComponent->BindAction("Spray", IE_Released, this, &APlayerCharacter::UnShowSelectionWheel);
	PlayerInputComponent->BindAction("Click", IE_Released, this, &APlayerCharacter::Draw);

}

void APlayerCharacter::Forward(float Value)
{
	if (IsValid(GetController()) && (Value != 0.0f))
	{
		// Trouver o� est l'avant
		FRotator Rotation = GetController()->GetControlRotation();
		// Ne pas tenir compte du pitch
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// Ajouter le mouvement dans la direction Avant � construire le vecteur
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		if (GetInputAxisValue("Right") != 0)
			AddMovementInput(Direction, Value * Vitesse * sqrt(2)/2);
		else
			AddMovementInput(Direction, Value * Vitesse);
	}

}

void APlayerCharacter::Right(float Value)
{

	if (IsValid(GetController()) && (Value != 0.0f))
	{
		// Trouver o� est la droite
		const FRotator Rotation = GetController()->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// Ajouter le mouvement dans cette direction
		if (GetInputAxisValue("Forward") != 0)
			AddMovementInput(Direction, Value * Vitesse * sqrt(2)/2);
		else
			AddMovementInput(Direction, Value * Vitesse);
	}

}

bool APlayerCharacter::IsRunning()
{
	return bRunning;
}

void APlayerCharacter::OnStartRun()
{
	float velocityLenght = GetVelocity().Size();
	if (!bWaitFullRecovery && velocityLenght != 0)
	{
		Vitesse = RunSpeed;
		bRunning = true;
		LaunchStaminaConsume();
	}
}

void APlayerCharacter::OnStopRun()
{
	if (!bWaitFullRecovery && bRunning)
	{
		Vitesse = BaseSpeed;
		bRunning = false;
		LaunchStaminaRegen();
	}
}

void APlayerCharacter::LaunchStaminaRegen()
{
	GetWorld()->GetTimerManager().ClearTimer(timerHandle);
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &APlayerCharacter::RegenStamina, 0.75, true, 1);
}

void APlayerCharacter::LaunchStaminaConsume()
{
	GetWorld()->GetTimerManager().ClearTimer(timerHandle);
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &APlayerCharacter::ConsumeStamina, 0.5, true);
}

void APlayerCharacter::ShowSelectionWheel()
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

void APlayerCharacter::UnShowSelectionWheel()
{
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());

	if (IsValid(playerController) && playerController->IsLocalController() && playerController->bHasChalk && IsValid(playerController->SelectionWheel))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Removed Selection wheel"));
		playerController->SetIgnoreLookInput(false);
		playerController->bShowMouseCursor = false;
		playerController->SelectionWheel->RemoveFromViewport();
		playerController->SetInputMode(FInputModeGameOnly());
	}

}

void APlayerCharacter::Draw()
{
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());

	if (IsValid(playerController) && playerController->IsLocalController() && playerController->bHasChalk && IsValid(playerController->SelectionWheel) && playerController->SelectionWheel->IsInViewport())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Drew something"));
		float Angle = Cast<USelectionWheelUserWidget>(playerController->SelectionWheel)->GetAngle();

		if (Angle >= -180 && Angle <= 180)
		{
			TypeDraw sprayType = TypeDraw::FRONT_ARROW;

			if (Angle < 30.f && Angle > -30.f)
				sprayType = TypeDraw::QUESTION_MARK;
			else if (Angle > 30.f && Angle < 90.f)
				sprayType = TypeDraw::CIRCLE;
			else if (Angle > 90.f && Angle < 145.f)
				sprayType = TypeDraw::RIGHT_ARROW;
			else if (Angle > 145.f && Angle < -145.f)
				sprayType = TypeDraw::FRONT_ARROW;
			else if (Angle < -90.f && Angle > -145.f)
				sprayType = TypeDraw::LEFT_ARROW;
			else if (Angle < -30.f && Angle > -90.f)
				sprayType = TypeDraw::CROSS;

			FHitResult hitResult = GetPositionInView();
			FTransform transf = { FQuat{}, hitResult.Location, hitResult.Normal };
			FVector pos = transf.GetLocation();
			AActor* hitres = hitResult.GetActor();

			if (Cast<AChalkDrawDecalActor>(hitResult.GetActor())) {
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString("Il y a deja un spray ici"));
				pos = hitResult.GetActor()->GetActorLocation();
				ServerClear(hitres);

			}



			// Limit of chalk : how far can the center of the spray be set?
			// Also making sure that we're not spraying the void.
			if ((!(FVector::Distance(transf.GetLocation(), GetActorLocation()) >= 250.f || FVector::Distance(pos, FVector{ 0, 0, 0 }) <= 1e-1)) && Cast<USelectionWheelUserWidget>(playerController->SelectionWheel)->GetHasMoved())
			{
				FVector normale = transf.GetLocation() - GetActorLocation();

				FVector right = -GetActorRightVector();
				FVector up = GetActorForwardVector();
				FRotator sprayRotation = UKismetMathLibrary::MakeRotationFromAxes(-normale, right, up);
				DrawDebugLine(GetWorld(), GetActorLocation(), pos, FColor::Blue, true);
				ServerSpray(sprayType, pos, sprayRotation);
			}
			UnShowSelectionWheel();

		}
	}
}


bool APlayerCharacter::ServerSpray_Validate(TypeDraw sprayType, FVector pos, FRotator sprayRotation) {
	return true;
}

void APlayerCharacter::ServerSpray_Implementation(TypeDraw sprayType, FVector pos, FRotator sprayRotation) {

	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());
	if (IsValid(playerController))
	{

		float sizeScale = 40.f;

		AActor* actor = InstanceBP(TEXT("/Game/Blueprints/Spray_BP.Spray_BP")
			, pos, sprayRotation);
		actor->SetActorScale3D({ sizeScale,sizeScale,sizeScale });

		AChalkDrawDecalActor* decal = Cast<AChalkDrawDecalActor>(actor);


		decal->kind = sprayType;
		//DrawDebugLine(GetWorld(), decal->GetActorLocation(), decal->GetActorLocation() + decal->GetActorForwardVector()*100, FColor::Blue, true, -1.0F, '\000',10.F);
		//DrawDebugLine(GetWorld(), decal->GetActorLocation(), decal->GetActorLocation() + decal->GetActorRightVector()*100, FColor::Orange, true, -1.0F, '\000', 10.F);
		//DrawDebugLine(GetWorld(), decal->GetActorLocation(), decal->GetActorLocation() + decal->GetActorUpVector()*100, FColor::Silver, true, -1.0F, '\000', 10.F);

	}
}

bool APlayerCharacter::ServerClear_Validate(AActor* acteur)
{
	return true;
}

void APlayerCharacter::ServerClear_Implementation(AActor* acteur)
{
	acteur->Destroy();
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*DOREPLIFETIME(APlayerCharacter, bWaitFullRecovery);
	DOREPLIFETIME(APlayerCharacter, stamina);*/
}

void APlayerCharacter::RegenStamina()
{
	if (stamina < staminaMax)
	{
		stamina++;
		if (stamina == 10 && bWaitFullRecovery)
			bWaitFullRecovery = false;
	}
		

}

void APlayerCharacter::ConsumeStamina()
{
	if (stamina > 0)
	{
		stamina--;
		if (stamina == 0)
		{
			OnStopRun();
			bWaitFullRecovery = true;
		}
	}
		
}

