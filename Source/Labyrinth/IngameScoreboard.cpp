// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameScoreboard.h"
#include "LabyrinthPlayerController.h"
#include "LabyrinthGameModeBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint\WidgetBlueprintLibrary.h"

UIngameScoreboard::UIngameScoreboard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

    ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFinder_Background(TEXT("/Game/Assets/Ingame_Scoreboard/Pseudo_Background.Pseudo_Background"));
    ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFinder_Lantern(TEXT("/Game/Assets/Ingame_Scoreboard/Lantern.Lantern"));
    ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFinder_Key(TEXT("/Game/Assets/Ingame_Scoreboard/Key.Key"));
    ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFinder_Trap(TEXT("/Game/Assets/Ingame_Scoreboard/Trap.Trap"));
    ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFinder_Chalk(TEXT("/Game/Assets/Ingame_Scoreboard/Chalk.Chalk"));

    textureBackground = ObjectFinder_Background.Object;
    textureLantern = ObjectFinder_Lantern.Object;
    textureKey = ObjectFinder_Key.Object;
    textureTrap = ObjectFinder_Trap.Object;
    textureChalk = ObjectFinder_Chalk.Object;
}

void UIngameScoreboard::GetPlayersInfo_Implementation()
{

    playersNames.Empty();
    playersInventories.Empty();

    ALabyrinthGameModeBase* gmb = Cast<ALabyrinthGameModeBase>(UGameplayStatics::GetGameMode(owner->GetWorld()));
    TArray<APlayerController*> apc = gmb->AllPlayerControllers;
    for (int i = 0; i < apc.Num(); i++) {
        playersNames.Add(Cast<ALabyrinthPlayerController>(apc[i])->playerSettings.PlayerName);
        TArray<bool> stats;
        stats.Add((Cast<ALabyrinthPlayerController>(apc[i]))->bHasLantern);
        stats.Add((Cast<ALabyrinthPlayerController>(apc[i]))->bHasChalk);
        stats.Add((Cast<ALabyrinthPlayerController>(apc[i]))->bHasKey);
        stats.Add((Cast<ALabyrinthPlayerController>(apc[i]))->bHasTrap);
        stats.Add((Cast<ALabyrinthPlayerController>(apc[i]))->bIsDead);
        playersInventories.Add(stats);
    }

    /*if (playersNames.Num() > 0)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Pseudos récupérés.");
    else
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Echec de récupération des infos.");*/
}

bool UIngameScoreboard::GetPlayersInfo_Validate() {
    return true;
}

int  UIngameScoreboard::GetNumberOfItems(int numPlayer) {
    if (numPlayer <= playersInventories.Num() - 1)
        return (playersInventories[numPlayer][0] + playersInventories[numPlayer][1] + playersInventories[numPlayer][2] + playersInventories[numPlayer][3]);
    else return 0;
}

int UIngameScoreboard::GetNumberOfPlayers() {
    return playersNames.Num();
}

FText UIngameScoreboard::GetPlayerName(int playerNumber) {
    if (playerNumber <= playersNames.Num() - 1)
        return playersNames[playerNumber];
    else return FText::AsCultureInvariant("");
}

TArray<bool> UIngameScoreboard::GetPlayerInventory(int playerNumber)
{
    if (playerNumber <= playersNames.Num() - 1)
        return playersInventories[playerNumber];
    else return TArray<bool>();
}

uint32 UIngameScoreboard::GetItemType(int playerNumber, int itemNumber)
{
    // LCKT
    TArray<bool> inventory = GetPlayerInventory(playerNumber);
    if (inventory.Num() == 0) return -1;

    TArray<uint32> newInventory{};
    for (int i = 0; i < 4; i++)
        if (inventory[i]) newInventory.Add(i);

    if (itemNumber > newInventory.Num()-1) return -1;
    else return newInventory[itemNumber];
}

FSlateBrush UIngameScoreboard::GetImage(int playerNumber, int itemNumber)
{
    uint32 itemType = GetItemType(playerNumber, itemNumber);
    switch (itemType) {
        case 0: return UWidgetBlueprintLibrary::MakeBrushFromTexture(textureLantern);
        case 1: return UWidgetBlueprintLibrary::MakeBrushFromTexture(textureChalk);
        case 2: return UWidgetBlueprintLibrary::MakeBrushFromTexture(textureKey);
        case 3: return UWidgetBlueprintLibrary::MakeBrushFromTexture(textureTrap);
        default: return {};
    }
}

bool UIngameScoreboard::GetIsAlive(int playerNumber)
{
    if (playerNumber <= playersNames.Num() - 1)
        return !playersInventories[playerNumber][4];
    else return false;
}
