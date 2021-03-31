#include "MainMenuLevelScriptActor.h"
#include "LabyrinthGameInstance.h"

void AMainMenuLevelScriptActor::BeginPlay() {

	ULabyrinthGameInstance* gameInstance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	gameInstance->SaveGameCheck();
}