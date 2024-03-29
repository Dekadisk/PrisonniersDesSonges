#include "SelectionWheelUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint\WidgetBlueprintLibrary.h"

USelectionWheelUserWidget::USelectionWheelUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

    ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFinder_SWCircle(TEXT("/Game/Assets/SelectionWheel/SelectionWheel_Circle.SelectionWheel_Circle"));
    ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFinder_SWCross(TEXT("/Game/Assets/SelectionWheel/SelectionWheel_Cross.SelectionWheel_Cross"));
    ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFinder_SWInterrogation(TEXT("/Game/Assets/SelectionWheel/SelectionWheel_InterrogationMark.SelectionWheel_InterrogationMark"));
    ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFinder_SWLeft(TEXT("/Game/Assets/SelectionWheel/SelectionWheel_Left.SelectionWheel_Left"));
    ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFinder_SWRight(TEXT("/Game/Assets/SelectionWheel/SelectionWheel_Right.SelectionWheel_Right"));
    ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFinder_SWForward(TEXT("/Game/Assets/SelectionWheel/SelectionWheel_Forward.SelectionWheel_Forward"));
    ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFinder_SWBase(TEXT("/Game/Assets/SelectionWheel/SelectionWheel2.SelectionWheel2"));

    textureCircle = ObjectFinder_SWCircle.Object;
    textureCross = ObjectFinder_SWCross.Object;
    textureQuestion = ObjectFinder_SWInterrogation.Object;
    textureLeft = ObjectFinder_SWLeft.Object;
    textureRight = ObjectFinder_SWRight.Object;
    textureForward = ObjectFinder_SWForward.Object;
    textureBase = ObjectFinder_SWBase.Object;
}

void USelectionWheelUserWidget::NativeConstruct() {
    
    APlayerController* PC = GetOwningPlayer();
    if (PC)
        PC->GetViewportSize(sizeX, sizeY);

    PC->SetMouseLocation(sizeX / 2, sizeY / 2);
}

FReply USelectionWheelUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    APlayerController* PC = GetOwningPlayer();
    if (PC)
        PC->GetViewportSize(sizeX, sizeY);

    cX = sizeX / 2;
    cY = sizeY / 2;
    xvertical = 0;
    yvertical = -cY;
    vert = FVector2D(xvertical, yvertical);

    FVector2D pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
    pos = pos * UWidgetLayoutLibrary::GetViewportScale(this);
    
    FString coords = "Mouse Coords: " + FString::SanitizeFloat(pos.X) + " " + FString::SanitizeFloat(pos.Y);
    FString coordsCtr = "Ctr Coords: " + FString::SanitizeFloat(cX) + " " + FString::SanitizeFloat(cY);
    FString coordsCtrS = "Taille Viewport: " + FString::SanitizeFloat(cX*2) + " " + FString::SanitizeFloat(cY*2);
    FString posp = FString::SanitizeFloat(xvertical) + " " + FString::SanitizeFloat(yvertical);
    FVector2D vertMouse = FVector2D(pos.X - cX, pos.Y - cY);
    FString coordsMouse = "V. Mouse/Ctr: " + FString::SanitizeFloat(vertMouse.X) + " " + FString::SanitizeFloat(vertMouse.Y);

    bHasMoved = abs(vertMouse.X) >= 10 || abs(vertMouse.Y) >= 10;

    float Ang1 = FMath::Atan2(vert.X, vert.Y);
    float Ang2 = FMath::Atan2(vertMouse.X, vertMouse.Y);
    Ang = FMath::RadiansToDegrees(Ang1 - Ang2);
    if (Ang > 180.0f) Ang -= 360.0f; else if (Ang < -180.0f) Ang += 360.0f;
    return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

bool USelectionWheelUserWidget::GetHasMoved() {
    return bHasMoved;
}

FSlateBrush USelectionWheelUserWidget::UpdateImage()
{
    if (!bHasMoved)
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(textureBase);
    if (Ang < 30.f && Ang > - 30.f)
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(textureQuestion);
    else if (Ang > 30.f && Ang < 90.f)
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(textureCircle);
    else if (Ang > 90.f && Ang < 145.f)
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(textureRight);
    else if (Ang > -90.f && Ang < -30.f)
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(textureCross);
    else if (Ang < -90.f && Ang > -145.f)
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(textureLeft);
    else
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(textureForward);
}

float USelectionWheelUserWidget::GetAngle() {
    return Ang;
}