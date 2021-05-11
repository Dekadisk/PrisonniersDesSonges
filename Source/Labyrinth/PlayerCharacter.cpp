#include "PlayerCharacter.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "LabyrinthPlayerController.h"
#include "SelectionWheelUserWidget.h"
#include "PickUpActor.h"
#include "UsableActor.h"
#include "TrapActor.h"
#include "ChalkDrawDecalActor.h"
#include "IngameScoreboard.h"

#include "MonsterCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LabyrinthGameModeBase.h"
#include "DeathSpectatorPawn.h"

bool APlayerCharacter::shouldUseAlternativeInfluence()
{
	return false;
}

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

	PlayerInputComponent->BindAction("ShowScoreboard", IE_Pressed, this, &APlayerCharacter::ShowScoreboard);
	PlayerInputComponent->BindAction("ShowScoreboard", IE_Released, this, &APlayerCharacter::UnShowScoreboard);

	PlayerInputComponent->BindAction("SetTrap", IE_Released, this, &APlayerCharacter::SetTrap);

}



void APlayerCharacter::Forward(float Value)
{
	if (IsValid(GetController()) && (Value != 0.0f))
	{
		// Trouver o� est l'avant
		FRotator Rotation = GetController()->GetControlRotation();
		
		// Ne pas tenir compte du pitch
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
			Rotation.Pitch = 0.0f;

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

void APlayerCharacter::UsainBolt()
{
	const_cast<float&>(BaseSpeed) = 1.0f;
	Vitesse = BaseSpeed;
}

void APlayerCharacter::GiveKey()
{
	Cast<ALabyrinthPlayerController>(GetController())->bHasKey = true;
}

void APlayerCharacter::IAmBatman(int val) {
	if (val)
		ServerHide();
	else
		ServerUnhide();
}

void APlayerCharacter::ShowSelectionWheel()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Selection wheel shown"));
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());

	if (!playerController->chatOn) {
		if (IsValid(playerController) && playerController->IsLocalController() && playerController->bHasChalk && IsValid(playerController->SelectionWheel))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Selection wheel shown"));
			playerController->SetIgnoreLookInput(true);
			playerController->bShowMouseCursor = true;
			playerController->SelectionWheel->AddToViewport();
			playerController->SetInputMode(FInputModeGameAndUI());
		}
	}
}

void APlayerCharacter::UnShowSelectionWheel()
{
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());

	if (!playerController->chatOn) {
		if (IsValid(playerController) && playerController->IsLocalController() && playerController->bHasChalk && IsValid(playerController->SelectionWheel))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Removed Selection wheel"));
			playerController->SetIgnoreLookInput(false);
			playerController->bShowMouseCursor = false;
			playerController->SelectionWheel->RemoveFromViewport();
			playerController->SetInputMode(FInputModeGameOnly());
		}
	}
}

void APlayerCharacter::ShowScoreboard()
{
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());
	Cast<UIngameScoreboard>(playerController->Scoreboard)->GetPlayersInfo();
	playerController->Scoreboard->AddToViewport();
}

void APlayerCharacter::UnShowScoreboard()
{
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());
	playerController->Scoreboard->RemoveFromViewport();
}

void APlayerCharacter::SetTrap()
{
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());
	if (IsValid(playerController) && playerController->IsLocalController() && playerController->bHasTrap)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Setting up a trap!"));

		FHitResult hitResult = GetPositionInView();
		FTransform transf = { FQuat{}, hitResult.Location, hitResult.Normal };
		FVector pos = transf.GetLocation();
		AActor* hitres = hitResult.GetActor();

		// Test that we're putting it on the ground instead. Test location!
		if (hitres->IsA(APickUpActor::StaticClass()) || hitres->IsA(AUsableActor::StaticClass()) || hitres->IsA(APlayerCharacter::StaticClass()) || hitres->IsA(AMonsterCharacter::StaticClass())) {
			return;
		}

		if (!(FVector::Distance(transf.GetLocation(), GetActorLocation()) >= 150.f || FVector::Distance(pos, FVector{ 0, 0, 0 }) <= 1e-1)) {
			FVector forward;
			FRotator sprayRotation;

			FVector normale = transf.GetLocation() - GetActorLocation();
			FVector right = -GetActorRightVector();

			if (abs(FVector::DotProduct(-transf.GetScale3D(), GetActorUpVector())) >= 0.5)
				forward = GetActorForwardVector();
			else
				forward = GetActorUpVector();

			sprayRotation = UKismetMathLibrary::MakeRotationFromAxes(transf.GetScale3D(), right, forward);
			playerController->bHasTrap = false;
			ServerSetTrap(pos, sprayRotation);
		}

	}
}

void APlayerCharacter::Draw()
{
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());

	if (IsValid(playerController) && playerController->IsLocalController() && playerController->bHasChalk && IsValid(playerController->SelectionWheel) && playerController->SelectionWheel->IsInViewport())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Drew something"));
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
			FVector oldForward;
			bool bIsReplacement{ false };

			//DrawDebugLine(GetWorld(), transf.GetLocation(), transf.GetLocation() + FVector{ 10,10,10 }, FColor::Purple, true);
			//DrawDebugLine(GetWorld(), transf.GetLocation(), transf.GetLocation() + FVector{ 10,10,-10 }, FColor::Purple, true);

			if (hitres->IsA(APickUpActor::StaticClass()) || hitres->IsA(AUsableActor::StaticClass()) || hitres->IsA(APlayerCharacter::StaticClass()) || hitres->IsA(AMonsterCharacter::StaticClass())) {
				UnShowSelectionWheel();
				return;
			}

			if (Cast<AChalkDrawDecalActor>(hitResult.GetActor())) {
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString("Il y a deja un spray ici"));
				pos = hitResult.GetActor()->GetActorLocation();
				
				bIsReplacement = true;
				if ((!(FVector::Distance(transf.GetLocation(), GetActorLocation()) >= 150.f || FVector::Distance(pos, FVector{ 0, 0, 0 }) <= 1e-1)) && Cast<USelectionWheelUserWidget>(playerController->SelectionWheel)->GetHasMoved()) {
					oldForward = hitResult.GetActor()->GetActorForwardVector();
					ServerClear(hitres);
				}

			}



			// Limit of chalk : how far can the center of the spray be set?
			// Also making sure that we're not spraying the void.
			if ((!(FVector::Distance(transf.GetLocation(), GetActorLocation()) >= 150.f || FVector::Distance(pos, FVector{ 0, 0, 0 }) <= 1e-1)) && Cast<USelectionWheelUserWidget>(playerController->SelectionWheel)->GetHasMoved())
			{
				FVector forward;
				FRotator sprayRotation;

				FVector normale = transf.GetLocation() - GetActorLocation();
				FVector right = -GetActorRightVector();

				if (abs(FVector::DotProduct(-transf.GetScale3D(), GetActorUpVector())) >= 0.5)
					forward = GetActorForwardVector();
				else
					forward = GetActorUpVector();
					
				sprayRotation = bIsReplacement ? 
							    UKismetMathLibrary::MakeRotationFromAxes(oldForward, right, forward) 
							  : UKismetMathLibrary::MakeRotationFromAxes(transf.GetScale3D(), right, forward);

				ServerSpray(sprayType, pos, sprayRotation);
					
				//DrawDebugLine(GetWorld(), GetActorLocation(), pos, FColor::White, true);
				//DrawDebugLine(GetWorld(), pos, pos + right * 10, FColor::Green, true, -1.0F, '\000', 1.F);
				//DrawDebugLine(GetWorld(), pos, pos + forward * 10, FColor::Red, true, -1.0F, '\000', 1.F);
			}
			UnShowSelectionWheel();

		}
	}
}

bool APlayerCharacter::ServerSetTrap_Validate(FVector pos, FRotator sprayRotation) {
	return true;
}

void APlayerCharacter::ServerSetTrap_Implementation(FVector pos, FRotator sprayRotation) {

	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());
	if (IsValid(playerController))
	{

		float sizeScale = 0.5f;
		pos = pos + FVector{0, 0, 10};
		AActor* actor = InstanceBP(TEXT("/Game/Blueprints/Trap_BP.Trap_BP")
			, pos, FRotator{ 0,0,0 }, { sizeScale,sizeScale,sizeScale });
		Cast<ATrapActor>(actor)->bIsOpen = true;
		//actor->SetActorScale3D({ sizeScale,sizeScale,sizeScale });
		//DrawDebugLine(GetWorld(), decal->GetActorLocation(), decal->GetActorLocation() + decal->GetActorForwardVector()*100, FColor::Blue, true, -1.0F, '\000',10.F);
		//DrawDebugLine(GetWorld(), decal->GetActorLocation(), decal->GetActorLocation() + decal->GetActorRightVector()*100, FColor::Orange, true, -1.0F, '\000', 10.F);
		//DrawDebugLine(GetWorld(), decal->GetActorLocation(), decal->GetActorLocation() + decal->GetActorUpVector()*100, FColor::Silver, true, -1.0F, '\000', 10.F);

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
		decal->OnRep_UpdateMaterial();
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

void APlayerCharacter::HandleDeath()
{
	auto controller = Cast<ALabyrinthPlayerController>(GetController());
	if (GetController()->IsValidLowLevel() && controller) {

		controller->bIsDead = true;
		if (HasAuthority()) {

			bool end = Cast<ALabyrinthGameModeBase>(GetWorld()->GetAuthGameMode())->EndGame();

			if (end)
				return;
			else {

				ADeathSpectatorPawn* spec = GetWorld()->SpawnActor<ADeathSpectatorPawn>(ADeathSpectatorPawn::StaticClass(), GetTransform());
				controller->Possess(spec);
				if (controller->pLantern)
					controller->pLantern->Destroy();
				Destroy();
				return;
			}
		}
	}
}

bool APlayerCharacter::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor) const
{
	static const FName NAME_AILineOfSight = FName(TEXT("PlayerLineOfSight"));

	FHitResult HitResult;
	const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, GetActorLocation()
		, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic))
		, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

	FVector socketLocation = GetMesh()->GetSocketLocation("Head");

	const bool bHitHead = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, socketLocation
		, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic))
		, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

	if (bHitHead == false) {
		OutSeenLocation = socketLocation;
		OutSightStrength = 1;

		return true;
	}

	socketLocation = GetMesh()->GetSocketLocation("LeftHand");

	const bool bHitLHand = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, socketLocation
		, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic))
		, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

	if (bHitLHand == false) {
		OutSeenLocation = socketLocation;
		OutSightStrength = 1;

		return true;
	}

	socketLocation = GetMesh()->GetSocketLocation("RightHand");

	const bool bHitRHand = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, socketLocation
		, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic))
		, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

	if (bHitRHand == false) {
		OutSeenLocation = socketLocation;
		OutSightStrength = 1;

		return true;
	}

	socketLocation = GetMesh()->GetSocketLocation("LeftFoot");

	const bool bHitLFoot = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, socketLocation
		, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic))
		, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

	if (bHitLFoot == false) {
		OutSeenLocation = socketLocation;
		OutSightStrength = 1;

		return true;
	}

	socketLocation = GetMesh()->GetSocketLocation("RightFoot");

	const bool bHitRFoot = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, socketLocation
		, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic))
		, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

	if (bHitRFoot == false) {
		OutSeenLocation = socketLocation;
		OutSightStrength = 1;

		return true;
	}

	if (bHit == false)
	{
		OutSeenLocation = GetActorLocation();
		OutSightStrength = 1;

		return true;
	}


	OutSightStrength = 0;
	return false;
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

//void ALabCharacter::Tick(float DeltaTime)
//{
//	//Super::Tick(DeltaTime);
//	
//}

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

void APlayerCharacter::ServerHide_Implementation() {
	UAIPerceptionSystem::GetCurrent(GetWorld())->UAIPerceptionSystem::UnregisterSource(*this, UAISense_Sight::StaticClass());
}

bool APlayerCharacter::ServerHide_Validate() {
	return true; 
}

void APlayerCharacter::ServerUnhide_Implementation() {
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
}

bool APlayerCharacter::ServerUnhide_Validate() {
	return true;
}