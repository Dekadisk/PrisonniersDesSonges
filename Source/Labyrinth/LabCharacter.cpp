#include "LabCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "UsableActor.h"
#include "Cachette.h"
#include "LabyrinthPlayerController.h"


// Sets default values
ALabCharacter::ALabCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MaxUseDistance = 400;

	UCapsuleComponent* capsule = GetCapsuleComponent();
	capsule->SetNotifyRigidBodyCollision(true);
	GetMesh()->SetNotifyRigidBodyCollision(true);

}

// Called every frame
void ALabCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Controller && Controller->IsLocalController())
	{
		AUsableActor* Usable = GetUsableInView();
		// Terminer le focus sur l'objet pr�c�dent
		if (FocusedUsableActor && FocusedUsableActor->IsA(ACachette::StaticClass())) {
			FHitResult hit = GetPositionInView();
			if (hit.GetActor() != nullptr && (hit.GetComponent()->GetName() != FString("PorteD_MESH") || hit.GetComponent()->GetName() != FString("PorteG_MESH"))) {
				FocusedUsableActor->OnEndFocus();
				bHasNewFocus = true;
			}
		}
		if (FocusedUsableActor != Usable)
		{
			if (FocusedUsableActor)
				FocusedUsableActor->OnEndFocus();
			bHasNewFocus = true;
		}
		// Assigner le nouveau focus (peut �tre nul )
		FocusedUsableActor = Usable;
		// D�marrer un nouveau focus si Usable != null;
		if (Usable)
		{
			if (bHasNewFocus)
			{
				if (Usable->IsA(ACachette::StaticClass())) {
					FHitResult hit = GetPositionInView();
					if (hit.GetComponent()->GetName() == FString("PorteD_MESH") || hit.GetComponent()->GetName() == FString("PorteG_MESH")) {
						Usable->OnBeginFocus();
						bHasNewFocus = false;
					}
				}
				else {
					Usable->OnBeginFocus();
					bHasNewFocus = false;
				}
			}
		}
	}
}

// Called to bind functionality to input
void ALabCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ALabCharacter::Use);
	PlayerInputComponent->BindAction("AlternativeUse", IE_Pressed, this, &ALabCharacter::AlternativeUse);
	PlayerInputComponent->BindAction("Chat", IE_Pressed, this, &ALabCharacter::Chat);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ALabCharacter::Pause);

}

void ALabCharacter::Use()
{
	if (HasAuthority())
	{
		
		AUsableActor* Usable = GetUsableInView();
		
		if (Usable)
		{
			FHitResult hit = GetPositionInView();
			if (hit.GetActor()->IsA(ACachette::StaticClass()) && (hit.GetComponent()->GetName() == FString("PorteD_MESH") || hit.GetComponent()->GetName() == FString("PorteG_MESH")))
				Usable->Use(false, this);
			else if (!hit.GetActor()->IsA(ACachette::StaticClass()))
				Usable->Use(false, this);
		}
	}
	else
	{
		ServerUse();
	}
}

void ALabCharacter::Trap_Implementation()
{
	bIsTrapped = true;
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());
	if (IsValid(playerController) && playerController->IsLocalController())
		playerController->SetIgnoreMoveInput(true);
}

void ALabCharacter::Untrap_Implementation()
{
	bIsTrapped = false;
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(GetController());
	if (IsValid(playerController) && playerController->IsLocalController())
		playerController->SetIgnoreMoveInput(false);
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
	if (mouseShown && pc->chatOn) {
		pc->chatOn = false;
		pc->ChatWidget->SetVisibility(ESlateVisibility::Hidden);
		pc->SetInputMode(FInputModeGameOnly());
	}
	else {
		pc->chatOn = true;
		pc->ChatWidget->SetVisibility(ESlateVisibility::Visible);
		pc->SetInputMode(FInputModeGameAndUI());
	}
}

void ALabCharacter::Pause() {

	ALabyrinthPlayerController* pc = Cast<ALabyrinthPlayerController>(GetController());
	bool mouseShown = pc->bShowMouseCursor;
	
	if (mouseShown && pc->pauseOn) {
		pc->bShowMouseCursor = !mouseShown;
		pc->pauseOn = false;
		pc->PauseWidget->RemoveFromParent();
		pc->SetInputMode(FInputModeGameOnly());
	}
	else if (mouseShown) {
		pc->pauseOn = true;
		pc->ShowPauseMenu();
		pc->ChatWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		pc->bShowMouseCursor = !mouseShown;
		pc->pauseOn = true;
		pc->ShowPauseMenu();
	}
}

AActor* ALabCharacter::InstanceBP(const TCHAR* bpName, FVector location, FRotator rotation, FVector scale)
{
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, bpName));

	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	if (!SpawnActor)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		return nullptr;
	}

	UClass* SpawnClass = SpawnActor->StaticClass();
	if (SpawnClass == NULL)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
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
	DOREPLIFETIME(ALabCharacter, bIsTrapped);

}

