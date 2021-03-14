// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionWheelUserWidget.h"

void USelectionWheelUserWidget::NativeOnInitialized() {
    APlayerController* PC = GetOwningPlayer();
    if (PC) {
        PC->GetViewportSize(sizeX, sizeY);
    }

    cX = sizeX / 2;
    cY = sizeY / 2;
    xvertical = 0;
    yvertical = -cY;
    vert = FVector2D(xvertical, yvertical);

}

FReply USelectionWheelUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FVector2D pos = InMouseEvent.GetScreenSpacePosition();

    FString coords = FString::SanitizeFloat(pos.X) + " " + FString::SanitizeFloat(pos.Y);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, coords);

    FString posp = FString::SanitizeFloat(xvertical) + " " + FString::SanitizeFloat(yvertical);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, posp);

    FVector2D vertMouse = FVector2D(pos.X - cX, pos.Y - cY);
    FString coordsMouse = "VertMouse : " + FString::SanitizeFloat(vertMouse.X) + " " + FString::SanitizeFloat(vertMouse.Y);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, coordsMouse);

    float Ang1 = FMath::Atan2(vert.X, vert.Y);
    float Ang2 = FMath::Atan2(vertMouse.X, vertMouse.Y);
    float Ang = FMath::RadiansToDegrees(Ang1 - Ang2);
    if (Ang > 180.0f) Ang -= 360.0f; else if (Ang < -180.0f) Ang += 360.0f;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(Ang));

    return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}
