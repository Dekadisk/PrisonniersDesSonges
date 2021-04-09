#include "ServerFoundUserWidget.h"
#include "ServerMenuUserWidget.h"

void UServerFoundUserWidget::OnClickedJoin() {
	Cast<UServerMenuUserWidget>(serverMenu)->Join(ServerName);
}