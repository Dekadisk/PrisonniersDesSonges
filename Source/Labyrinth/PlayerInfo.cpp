#include "PlayerInfo.h"

FPlayerInfo::FPlayerInfo() {
	//static ConstructorHelpers::FObjectFinder<UTexture2D> PlayerImageObj(TEXT("/Engine/VREditor/Devices/Vive/UE4_Logo"));
	//PlayerImage = PlayerImageObj.Object;

	PlayerStatus = FString("Prends une douche");
}