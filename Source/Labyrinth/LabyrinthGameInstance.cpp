// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyrinthGameInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ULabyrinthGameInstance::ULabyrinthGameInstance() {
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidget{ TEXT("/Game/UI/MainMenu") };
	MenuWidgetClass = MenuWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> HostMenuWidget{ TEXT("/Game/UI/HostMenu") };
	HostMenuWidgetClass = HostMenuWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> ServerMenuWidget{ TEXT("/Game/UI/ServerMenu") };
	ServerMenuWidgetClass = ServerMenuWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> OptionsMenuWidget{ TEXT("/Game/UI/OptionsMenu") };
	OptionsMenuWidgetClass = OptionsMenuWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> LoadingScreenWidget{ TEXT("/Game/UI/LoadingScreen") };
	LoadingScreenWidgetClass = LoadingScreenWidget.Class;
}

void ULabyrinthGameInstance::ShowMainMenu() {

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	MainMenu = CreateWidget<UUserWidget>(playerController, MenuWidgetClass);

	MainMenu->AddToViewport();

	playerController->SetShowMouseCursor(true);
}

void ULabyrinthGameInstance::ShowHostMenu() {

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	HostMenu = CreateWidget<UUserWidget>(playerController, HostMenuWidgetClass);

	HostMenu->AddToViewport();
}

void ULabyrinthGameInstance::ShowServerMenu() {

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	ServerMenu = CreateWidget<UUserWidget>(playerController, ServerMenuWidgetClass);

	ServerMenu->AddToViewport();
}

void ULabyrinthGameInstance::ShowOptionsMenu() {

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	OptionsMenu = CreateWidget<UUserWidget>(playerController, OptionsMenuWidgetClass);

	OptionsMenu->AddToViewport();
}

void ULabyrinthGameInstance::ShowLoadingScreen() {

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	LoadingScreen = CreateWidget<UUserWidget>(playerController, LoadingScreenWidgetClass);

	LoadingScreen->AddToViewport();
}

void ULabyrinthGameInstance::LaunchLobby() {

	ShowLoadingScreen();

	UGameplayStatics::OpenLevel(GetWorld(), "Lobby", true, "listen");
	FURL url = GetWorld()->URL;
}

void ULabyrinthGameInstance::JoinServer(FText ip) {

	ShowLoadingScreen();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	playerController->ClientTravel(ip.ToString(), TRAVEL_Absolute);
}

void ULabyrinthGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Creer une session

/*ULabyrinthGameInstance::ULabyrinthGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ULabyrinthGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &ULabyrinthGameInstance::OnStartOnlineGameComplete);
}

bool ULabyrinthGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLan, bool bIsPresence, int32 MaxNumPlayers) {

	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the Session Interface, so we can call the "CreateSession" function on it
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLan;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME, FString("firstMap"), EOnlineDataAdvertisementType::ViaOnlineService);

			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Our delegate should get called when this is complete (doesn't need to be successful!)
			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}

void ULabyrinthGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to call the StartSession function
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// Our StartSessionComplete delegate should get called after this
				Sessions->StartSession(SessionName);
			}
		}

	}
}

void ULabyrinthGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful) {

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the Online Subsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to clear the Delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "firstMap", true, "listen");
	}
}*/