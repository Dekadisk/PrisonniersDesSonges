#include "LabyrinthGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "LabyrinthGameModeBase.h"
#include "LabyrinthPlayerController.h"
#include "AkGameplayStatics.h"

ULabyrinthGameInstance::ULabyrinthGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidget{ TEXT("/Game/UI/MainMenu") };
	MenuWidgetClass = MenuWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> HostMenuWidget{ TEXT("/Game/UI/HostMenu") };
	HostMenuWidgetClass = HostMenuWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> ServerMenuWidget{ TEXT("/Game/UI/ServerMenu") };
	ServerMenuWidgetClass = ServerMenuWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> LeaderBoardWidget{ TEXT("/Game/UI/LeaderBoardMenu") };
	LeaderBoardWidgetClass = LeaderBoardWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> OptionsMenuWidget{ TEXT("/Game/UI/OptionsMenu") };
	OptionsMenuWidgetClass = OptionsMenuWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> NameMenuWidget{ TEXT("/Game/UI/NameMenu") };
	NameMenuWidgetClass = NameMenuWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> LoadingScreenWidget{ TEXT("/Game/UI/LoadingScreen") };
	LoadingScreenWidgetClass = LoadingScreenWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> TitleScreenWidget{ TEXT("/Game/UI/TitleScreen") };
	TitleScreenWidgetClass = TitleScreenWidget.Class;

	SaveName = "PlayerSettingsSaved";

	save = NewObject<UPlayerSaveGame>();

	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ULabyrinthGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &ULabyrinthGameInstance::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &ULabyrinthGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ULabyrinthGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &ULabyrinthGameInstance::OnDestroySessionComplete);

	currentPartyDataForSave = NewObject<UParty>();
	currentPartyDataForSave->seedUsed = 0;
	currentPartyDataForSave->serverName = "Party Bidon";
	currentPartyDataForSave->nbSurvivor = 3;
	currentPartyDataForSave->partyDuration = 0;
}


void ULabyrinthGameInstance::ShowMainMenu()
{
	if (IsValid(LoadingScreen))
	{
		if(LoadingScreen->IsInViewport())
			LoadingScreen->RemoveFromViewport();
		LoadingScreen->Destruct();
		LoadingScreen = nullptr;
	}

	if (IsValid(TitleScreen))
	{
		TitleScreen->RemoveFromViewport();
		TitleScreen->Destruct();
		TitleScreen = nullptr;
	}

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

void ULabyrinthGameInstance::ShowLeaderBoardMenu()
{
	if (IsValid(LoadingScreen))
	{
		LoadingScreen->RemoveFromViewport();
		LoadingScreen->Destruct();
		LoadingScreen = nullptr;
	}
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	LeaderBoardMenu = CreateWidget<UUserWidget>(playerController, LeaderBoardWidgetClass);

	LeaderBoardMenu->AddToViewport();
}

void ULabyrinthGameInstance::ShowOptionsMenu() {

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	OptionsMenu = CreateWidget<UUserWidget>(playerController, OptionsMenuWidgetClass);

	OptionsMenu->AddToViewport();
}

void ULabyrinthGameInstance::ShowNameMenu()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	NameMenu = CreateWidget<UUserWidget>(playerController, NameMenuWidgetClass);

	NameMenu->AddToViewport();
}

void ULabyrinthGameInstance::ShowLoadingScreen() {

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	LoadingScreen = CreateWidget<UUserWidget>(playerController, LoadingScreenWidgetClass);

	LoadingScreen->AddToViewport();
}

void ULabyrinthGameInstance::ShowTitleScreen()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	TitleScreen = CreateWidget<UUserWidget>(playerController, TitleScreenWidgetClass);
	TitleScreen->AddToViewport();
	playerController->bShowMouseCursor = true;
	onTitle = true;
}

void ULabyrinthGameInstance::ShowLoadingScreen(APlayerController* playerController)
{
	LoadingScreen = CreateWidget<UUserWidget>(playerController, LoadingScreenWidgetClass);
	LoadingScreen->AddToViewport();
}

void ULabyrinthGameInstance::LaunchLobby(int32 nbPlayers, bool lan, FName _ServerName) {

	ShowLoadingScreen();
	currentPartyDataForSave = NewObject<UParty>();
	currentPartyDataForSave->maxPlayers = nbPlayers;
	currentPartyDataForSave->serverName = _ServerName.ToString();

	HostSession(GetPrimaryPlayerUniqueId(), _ServerName, lan, false, nbPlayers, FText::FromString(StartingLevel));
}

void ULabyrinthGameInstance::JoinServer(FName _SessionName, FOnlineSessionSearchResult SessionToJoin) {

	ShowLoadingScreen();

	JoinSession(GetPrimaryPlayerUniqueId(), _SessionName, SessionToJoin);
}

void ULabyrinthGameInstance::SaveGameCheck()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0)) {

		save = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
		ExecOptions();
		fileSaved = true;
	}
	else {
		//auto res = GEngine->GetGameUserSettings()->GetDesktopResolution();
		
		GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Fullscreen);	
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand("r.setRes 1920x1080f");
		GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();
		UGameplayStatics::GetPlayerController(GetWorld(),0)->SetShowMouseCursor(true);
	}
}

void ULabyrinthGameInstance::OnClickTitleScreen()
{
	if (fileSaved) {
		if (!save->GetPlayerInfo().PlayerName.IsEmpty()) {
			LoginOnScoreServer();
			ShowLoadingScreen();
		}
		else {
			ShowNameMenu();
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
		}
	}
	else {
		ShowNameMenu();
	}
	onTitle = false;
}

void ULabyrinthGameInstance::ExecOptions() {

	if (save->GetPlayerInfo().Fullscreen) {
		GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Fullscreen);
		GEngine->GetGameUserSettings()->ApplyNonResolutionSettings();

		FString exeResolution = "r.setRes " + save->GetPlayerInfo().Resolution.ToString() + "f";
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand(exeResolution);
	}
	else {
		GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
		GEngine->GetGameUserSettings()->ApplySettings(false);

		FString exeResolution = "r.setRes " + save->GetPlayerInfo().Resolution.ToString() + "w";
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand(exeResolution);
	}
	

	FString exeShadow = "sg.ShadowQuality " + save->GetPlayerInfo().ShadowQuality.ToString();
	UGameplayStatics::GetPlayerController(GetWorld(),0)->ConsoleCommand(exeShadow);

	FString exeTexture = "sg.TextureQuality " + save->GetPlayerInfo().TextureQuality.ToString();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand(exeTexture);

	FString exePost = "sg.PostProcessQuality " + save->GetPlayerInfo().PostQuality.ToString();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand(exePost);

	FString exe = save->GetPlayerInfo().Language.ToString() == "Francais" ? "fr-FR" : "en-GB";
	UKismetInternationalizationLibrary::SetCurrentCulture(exe);
	UKismetInternationalizationLibrary::SetCurrentLanguage(exe);

	UAkGameplayStatics::SetRTPCValue(FName("MasterVolume"), save->GetPlayerInfo().MasterVolume, 0.0f, nullptr);
	UAkGameplayStatics::SetRTPCValue(FName("MusicVolume"), save->GetPlayerInfo().MusicVolume, 0.0f, nullptr);
	UAkGameplayStatics::SetRTPCValue(FName("SFXVolume"), save->GetPlayerInfo().SFXVolume, 0.0f, nullptr);
}

// Creer une session

bool ULabyrinthGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName _SessionName, bool bIsLan, bool bIsPresence, int32 MaxNumPlayers, FText MapName) {

	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the Session Interface, so we can call the "CreateSession" function on it
		auto Sessions = Online::GetSessionInterface();
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

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
			//
			SessionSettings->Set(SETTING_MAPNAME, MapName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);
			SessionSettings->Set(SETTING_CUSTOMSEARCHINT1, _SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);
			SessionSettings->Set(SETTING_CUSTOMSEARCHINT2, FString::FromInt(MaxNumPlayers - 1), EOnlineDataAdvertisementType::ViaOnlineService);

			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Our delegate should get called when this is complete (doesn't need to be successful!)
			return Sessions->CreateSession(*UserId, _SessionName, *SessionSettings);
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}

void ULabyrinthGameInstance::OnCreateSessionComplete(FName _SessionName, bool bWasSuccessful) {

	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *_SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to call the StartSession function
		auto Sessions = Online::GetSessionInterface();
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// Our StartSessionComplete delegate should get called after this
				Sessions->StartSession(_SessionName);
				SessionName = _SessionName;
			}
		}

	}
}

void ULabyrinthGameInstance::OnStartOnlineGameComplete(FName _SessionName, bool bWasSuccessful) {

	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *_SessionName.ToString(), bWasSuccessful));

	// Get the Online Subsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to clear the Delegate
		auto Sessions = Online::GetSessionInterface();
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	if (bWasSuccessful)
	{
		/*if (IsValid(LoadingScreen))
		{
			LoadingScreen->RemoveFromViewport();
			LoadingScreen->Destruct();
			LoadingScreen = nullptr;
		}*/
		UGameplayStatics::OpenLevel(GetWorld(), FName(StartingLevel), true, "listen");
	}
}

// Trouver une session

void ULabyrinthGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the SessionInterface from our OnlineSubsystem
		auto Sessions = Online::GetSessionInterface();
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*
				Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
			*/
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;
			SessionSearch->TimeoutInSeconds = 0;

			// We only want to set this Query Setting if "bIsPresence" is true
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			// Set the Delegate to the Delegate Handle of the FindSession function
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			// Finally call the SessionInterface function. The Delegate gets called once this is finished
			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
	else
	{
		// If something goes wrong, just call the Delegate Function directly with "false".
		OnFindSessionsComplete(false);
	}
}

void ULabyrinthGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnFindSessionsComplete bSuccess: %d"), bWasSuccessful));

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface of the OnlineSubsystem
		auto Sessions = Online::GetSessionInterface();
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the Delegate handle, since we finished this call
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			// Just debugging the Number of Search results. Can be displayed in UMG or something later on
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

			// If we have found at least 1 session, we just going to debug them. You could add them to a list of UMG Widgets, like it is done in the BP version!
			//if (SessionSearch->SearchResults.Num() > 0)
			//{
			//	// "SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
			//	// This can be customized later on with your own classes to add more information that can be set and displayed
			//	for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
			//	{
			//		// OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
			//		// This is something you can't do in Blueprint for example!
			//		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
			//	}

			//	
			//}
		}
	}
}

// Rejoindre une session

bool ULabyrinthGameInstance::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName _SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	// Return bool
	bool bSuccessful = false;

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		auto Sessions = Online::GetSessionInterface();
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			// Set the Handle again
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			// Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
			// "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
			bSuccessful = Sessions->JoinSession(*UserId, _SessionName, SearchResult);
		}
	}

	return bSuccessful;
}

void ULabyrinthGameInstance::OnJoinSessionComplete(FName _SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *_SessionName.ToString(), static_cast<int32>(Result)));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		auto Sessions = Online::GetSessionInterface();
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate again
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			// Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
			// This is something the Blueprint Node "Join Session" does automatically!
			APlayerController* const PlayerController = GetFirstLocalPlayerController();

			// We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
			// String for us by giving him the SessionName and an empty String. We want to do this, because
			// Every OnlineSubsystem uses different TravelURLs
			FString TravelURL;
			//
			if (PlayerController && Sessions->GetResolvedConnectString(_SessionName, TravelURL))
			{
				/*if (IsValid(LoadingScreen))
				{
					LoadingScreen->RemoveFromViewport();
					LoadingScreen->Destruct();
					LoadingScreen = nullptr;
				}*/
				// Finally call the ClienTravel. If you want, you could print the TravelURL to see
				// how it really looks like
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
				SessionName = _SessionName;
				SessionSearch.Reset();
			}
		}
	}
}

bool ULabyrinthGameInstance::DestroySession(FName _SessionName)
{
	// Return bool
	bool bSuccessful = false;

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		auto Sessions = Online::GetSessionInterface();
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Set the Handle again
			OnDestroySessionCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			bSuccessful = Sessions->DestroySession(_SessionName);

		}
	}

	return bSuccessful;
}

// Detruire une session

void ULabyrinthGameInstance::OnDestroySessionComplete(FName _SessionName, bool bWasSuccessful)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *_SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the SessionInterface from the OnlineSubsystem
		auto Sessions = Online::GetSessionInterface();
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			// If it was successful, we just load another level (could be a MainMenu!)
			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "/Game/UI/Main", true);
				SessionName = "";
			}
		}
	}
}


/* BACK END */

void ULabyrinthGameInstance::LoginOnScoreServer()
{
	check(save);
	if (save->GetPlayerInfo().UserId.IsEmpty() || save->GetPlayerInfo().GuestToken.IsEmpty())
	{
		CreateUserOnScoreServer();
		return;
	}

	if (save->GetPlayerInfo().SessionToken.IsEmpty())
	{
		CreateSessionOnScoreServer();
		return;
	}

	RefreshSessionOnScoreServer();
}

void ULabyrinthGameInstance::CreateUserOnScoreServer()
{

	FString content = FString::Printf(TEXT("name=%s"),
		*FGenericPlatformHttp::UrlEncode(save->GetPlayerInfo().PlayerName.ToString()));

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ULabyrinthGameInstance::OnCreateUserCompleted);
	Request->SetHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->SetURL(API_ENDPOINT + "users");
	Request->SetContentAsString(content);
	Request->SetVerb("POST");
	Request->ProcessRequest();
}

void ULabyrinthGameInstance::CreateSessionOnScoreServer()
{

	FString content = FString::Printf(TEXT("guestToken=%s"),
		*FGenericPlatformHttp::UrlEncode(save->GetPlayerInfo().GuestToken));

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ULabyrinthGameInstance::OnCreateSessionCompleted);
	Request->SetHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->SetURL(API_ENDPOINT + "users/" + save->GetPlayerInfo().UserId + "/sessions/create");
	Request->SetContentAsString(content);
	Request->SetVerb("POST");
	Request->ProcessRequest();
}

void ULabyrinthGameInstance::RefreshSessionOnScoreServer()
{

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ULabyrinthGameInstance::OnRefreshSessionCompleted);
	FString authHeader = FString("Bearer " + save->GetPlayerInfo().SessionToken);
	Request->SetURL(API_ENDPOINT + "users/" + save->GetPlayerInfo().UserId + "/sessions/refresh");
	Request->SetHeader("Authorization", authHeader);
	Request->SetVerb("POST");
	Request->ProcessRequest();
}

void ULabyrinthGameInstance::ChangeDBNameOnScoreServer(FString newName)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ULabyrinthGameInstance::OnChangeDBNameCompleted);
	FString authHeader = FString("Bearer " + save->GetPlayerInfo().SessionToken);
	Request->SetURL(API_ENDPOINT + "users/" + save->GetPlayerInfo().UserId + "/name?value=" + newName);
	Request->SetHeader("Authorization", authHeader);
	Request->SetVerb("PUT");
	Request->ProcessRequest();
}

bool ULabyrinthGameInstance::IsOfflineMod()
{
	return offlineMod;
}

void ULabyrinthGameInstance::ResetWaitingInfo()
{
	waitingMoreInfo = true;
}

void ULabyrinthGameInstance::SetStartTime()
{
	currentPartyDataForSave->partyDuration = UGameplayStatics::GetTimeSeconds(GetWorld());
}

void ULabyrinthGameInstance::CaculatePartyDuration()
{
	currentPartyDataForSave->partyDuration = UGameplayStatics::GetTimeSeconds(GetWorld()) - currentPartyDataForSave->partyDuration;
}

void ULabyrinthGameInstance::CreatePartyDB()
{

	FString content = FString::Printf(TEXT("serverName=%s&nbSurvivor=%d&seed=%d&partyDurationInSecond=%lld"),
		*currentPartyDataForSave->serverName, currentPartyDataForSave->nbSurvivor, currentPartyDataForSave->seedUsed, currentPartyDataForSave->partyDuration);
	FString authHeader = FString("Bearer " + save->GetPlayerInfo().SessionToken);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ULabyrinthGameInstance::OnCreatePartyCompleted);
	Request->SetURL(API_ENDPOINT + "party");
	Request->SetHeader("Authorization", authHeader);
	Request->SetHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->SetContentAsString(content);
	Request->SetVerb("POST");
	Request->ProcessRequest();
}

void ULabyrinthGameInstance::GetBestPartyOfPlayer()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ULabyrinthGameInstance::OnGetBestPartyOfPlayerCompleted);
	Request->SetURL(API_ENDPOINT + "party/bestgame/" + save->GetPlayerInfo().UserId);
	Request->SetVerb("GET");
	Request->ProcessRequest();
}

void ULabyrinthGameInstance::AddPlayerToParty(FString partyId)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ULabyrinthGameInstance::OnAddPlayerToPartyCompleted);
	FString authHeader = FString("Bearer " + save->GetPlayerInfo().SessionToken);
	Request->SetURL(API_ENDPOINT + "party/" + partyId + "/addPlayer");
	Request->SetHeader("Authorization", authHeader);
	Request->SetVerb("PUT");
	Request->ProcessRequest();
}

void ULabyrinthGameInstance::GetTop10Party()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ULabyrinthGameInstance::OnGetTop10PartyCompleted);
	Request->SetURL(API_ENDPOINT + "party?start=0&end=9");
	Request->SetVerb("GET");
	Request->ProcessRequest();
}

void ULabyrinthGameInstance::OnCreateUserCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FPlayerInfo playerInfo = save->GetPlayerInfo();

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<FString>::Create(response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
			playerInfo.UserId = JsonObject->GetStringField(TEXT("id"));
			playerInfo.GuestToken = JsonObject->GetStringField(TEXT("guestToken"));
			save->SetPlayerInfo(playerInfo);
			UGameplayStatics::SaveGameToSlot(save, SaveName, 0);
		}
		LoginOnScoreServer();
	}
	else
	{
		offlineMod = true;
		ShowMainMenu();
	}
}

void ULabyrinthGameInstance::OnCreateSessionCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FPlayerInfo playerInfo = save->GetPlayerInfo();

		playerInfo.SessionToken = response->GetContentAsString();
		save->SetPlayerInfo(playerInfo);

		UGameplayStatics::SaveGameToSlot(save, SaveName, 0);
		offlineMod = false;
	}
	else
	{
		offlineMod = true;
	}
	ShowMainMenu();
}

void ULabyrinthGameInstance::OnRefreshSessionCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
	FPlayerInfo playerInfo = save->GetPlayerInfo();
	int i = response->GetResponseCode();
	if (bWasSuccessful)
	{

		playerInfo.SessionToken = response->GetContentAsString();
		save->SetPlayerInfo(playerInfo);
		UGameplayStatics::SaveGameToSlot(save, SaveName, 0);
		offlineMod = false;
		ShowMainMenu();
		
	}
	else
	{
		playerInfo.SessionToken.Empty();
		LoginOnScoreServer();
	}
}

void ULabyrinthGameInstance::OnChangeDBNameCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{

	}
	else
	{

	}
	ShowMainMenu();
}

void ULabyrinthGameInstance::OnCreatePartyCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
	ALabyrinthGameModeBase* gameMode = Cast<ALabyrinthGameModeBase>(GetWorld()->GetAuthGameMode());
	if (bWasSuccessful)
	{
		int i = response->GetResponseCode();
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<FString>::Create(response->GetContentAsString());
		FString partyId = "";
		int nbSurivants = 4;
		if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
			partyId = JsonObject->GetStringField(TEXT("partyId"));
			nbSurivants = JsonObject->GetIntegerField(TEXT("nbSurvivor"));
		}

		if (IsValid(gameMode) && partyId != "")
		{
			for (APlayerController* playerController : gameMode->AllPlayerControllers)
			{
				ALabyrinthPlayerController* labyrinthPlayerController = Cast<ALabyrinthPlayerController>(playerController);
				if (IsValid(labyrinthPlayerController))
				{
					labyrinthPlayerController->AddPlayerToPartyDB(partyId, nbSurivants);
				}
			}
		}
	}
	else
	{
		if (IsValid(gameMode)) {
			ALabyrinthPlayerController* server = nullptr;
			for (APlayerController* pc : gameMode->AllPlayerControllers) {
				ALabyrinthPlayerController* labPC = Cast<ALabyrinthPlayerController>(pc);
				if (labPC->GetNetMode() == ENetMode::NM_Client)
					labPC->PlayCutscene(currentPartyDataForSave->nbSurvivor);
				else
					server = labPC;
			}
			if (server)
				server->PlayCutscene(currentPartyDataForSave->nbSurvivor);
		}
	}
}

void ULabyrinthGameInstance::OnGetBestPartyOfPlayerCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
	auto i = response->GetResponseCode();
	if (bWasSuccessful && response->GetResponseCode() != 404)
	{
		bestGameResult = NewObject<UParty>();
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<FString>::Create(response->GetContentAsString());
		if (FJsonSerializer::Deserialize(Reader, JsonObject)) 
		{
			bestGameResult->leaderBoardPosition = JsonObject->GetIntegerField("item1");
			bestGameResult->partyId = (JsonObject->GetObjectField("item2"))->GetStringField("partyId");
			bestGameResult->serverName = (JsonObject->GetObjectField("item2"))->GetStringField("serverName");
			bestGameResult->nbSurvivor = (JsonObject->GetObjectField("item2"))->GetIntegerField("nbSurvivor");
			bestGameResult->partyDuration = (JsonObject->GetObjectField("item2"))->GetNumberField("partyDurationInSecond");
			bestGameResult->seedUsed = (JsonObject->GetObjectField("item2"))->GetIntegerField("seed");

			/* G�rer les listes */
			for(auto it : (JsonObject->GetObjectField("item2"))->GetArrayField("playersName"))
			{
				bestGameResult->listPlayerName.Add(it->AsString());
			}
		}
	}
	else
	{
		bestGameResult = nullptr;
	}
	if (!waitingMoreInfo)
	{
		ShowLeaderBoardMenu();
	}
	else
	{
		waitingMoreInfo = false;
	}
}

void ULabyrinthGameInstance::OnAddPlayerToPartyCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		
	}
	else
	{

	}
}

void ULabyrinthGameInstance::OnGetTop10PartyCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
	top10Result = TArray<UParty*>();
	if (bWasSuccessful)
	{
		TArray<TSharedPtr<FJsonValue>> JsonArray;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<FString>::Create(response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonArray))
		{
			for (auto JsonObject : JsonArray)
			{
				auto object = JsonObject->AsObject();
				UParty* game = NewObject<UParty>();
				game->leaderBoardPosition = (object->GetObjectField("item1"))->GetIntegerField("rank");
				game->partyId = (object->GetObjectField("item2"))->GetStringField("partyId");
				game->serverName = (object->GetObjectField("item2"))->GetStringField("serverName");
				game->nbSurvivor = (object->GetObjectField("item2"))->GetIntegerField("nbSurvivor");
				game->partyDuration = (object->GetObjectField("item2"))->GetNumberField("partyDurationInSecond");
				game->seedUsed = (object->GetObjectField("item2"))->GetIntegerField("seed");

				/* G�rer les listes */
				for (auto it : (object->GetObjectField("item2"))->GetArrayField("playersName"))
				{
					game->listPlayerName.Add(it->AsString());
				}
				top10Result.Add(game);
			}

		}
	}
	else
	{

	}

	if (!waitingMoreInfo)
	{
		ShowLeaderBoardMenu();
	}
	else
	{
		waitingMoreInfo = false;
	}
}