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

int  UIngameScoreboard::GetNumberOfItems(int numPlayer) {
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Getting number of items.");
    if (numPlayer <= owner->playersNames.Num() - 1)
        switch (numPlayer) {
        case 0: return (owner->playersInventories1[0] + owner->playersInventories1[1] + owner->playersInventories1[2] + owner->playersInventories1[3]);
        case 1: return (owner->playersInventories2[0] + owner->playersInventories2[1] + owner->playersInventories2[2] + owner->playersInventories2[3]);
        case 2: return (owner->playersInventories3[0] + owner->playersInventories3[1] + owner->playersInventories3[2] + owner->playersInventories3[3]);
        case 3: return (owner->playersInventories4[0] + owner->playersInventories4[1] + owner->playersInventories4[2] + owner->playersInventories4[3]);
        default: return 0;
        }
    else return 0;
}

int UIngameScoreboard::GetNumberOfPlayers() {
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Getting number of players : "+ owner->playersNames.Num());
    return owner->playersNames.Num();
}

FText UIngameScoreboard::GetPlayerName(int playerNumber) {
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Getting players' names.");
    if (playerNumber <= owner->playersNames.Num() - 1)
        return owner->playersNames[playerNumber];
    else return FText::AsCultureInvariant("");
}

TArray<bool> UIngameScoreboard::GetPlayerInventory(int playerNumber)
{
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Getting players' inventories.");
    if (playerNumber <= owner->playersNames.Num() - 1) {
        switch (playerNumber) {
            case 0: return owner->playersInventories1;
            case 1: return owner->playersInventories2;
            case 2: return owner->playersInventories3;
            case 3: return owner->playersInventories4;
            default: return TArray<bool>();
        }
    }
    else return TArray<bool>();
}

uint32 UIngameScoreboard::GetItemType(int playerNumber, int itemNumber)
{
    // LCKT
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Getting item type.");
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
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Getting is alive.");
    if (playerNumber <= owner->playersNames.Num() - 1) {
        switch (playerNumber) {
            case 0: return !(owner->playersInventories1[4]);
            case 1: return !(owner->playersInventories2[4]);
            case 2: return !(owner->playersInventories3[4]);
            case 3: return !(owner->playersInventories4[4]);
            default: return false;
        }
    }
    else return false;
}
