#include "PlayerButtonUserWidget.h"
#include "LobbyGameMode.h"

void UPlayerButtonUserWidget::OnClickPlayer() {

	Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode())->ServerGetKicked(id);
	RemoveFromParent();
}

void UPlayerButtonUserWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayerButtonUserWidget, id);
	DOREPLIFETIME(UPlayerButtonUserWidget, playerInfo);
}