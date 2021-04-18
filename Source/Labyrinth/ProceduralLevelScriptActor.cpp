#include "ProceduralLevelScriptActor.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void AProceduralLevelScriptActor::BeginPlay() {
	Super::BeginPlay();

	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
}