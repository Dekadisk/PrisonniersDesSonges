// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionWheelUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include <Runtime\UMG\Public\Blueprint\WidgetBlueprintLibrary.h>

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

    selection_wheel_circle = FindObject<UTexture2D>(GetWorld(),TEXT("/Game/Assets/SelectionWheel_Circle"));
    selection_wheel_cross = FindObject<UTexture2D>(GetWorld(), TEXT("/Game/Assets/SelectionWheel_Cross"));
    selection_wheel_question = FindObject<UTexture2D>(GetWorld(), TEXT("/Game/Assets/SelectionWheel_InterrogationMark"));
    selection_wheel_left = FindObject<UTexture2D>(GetWorld(), TEXT("/Game/Assets/SelectionWheel_Left"));
    selection_wheel_right = FindObject<UTexture2D>(GetWorld(), TEXT("/Game/Assets/SelectionWheel_Right"));
    selection_wheel_straight = FindObject<UTexture2D>(GetWorld(), TEXT("/Game/Assets/SelectionWheel_Forward"));

}

FReply USelectionWheelUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    //FVector2D pos = InMouseEvent.GetScreenSpacePosition();
    FVector2D pos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
    FString coords = FString::SanitizeFloat(pos.X) + " " + FString::SanitizeFloat(pos.Y);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, coords);

    FString posp = FString::SanitizeFloat(xvertical) + " " + FString::SanitizeFloat(yvertical);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, posp);

    FVector2D vertMouse = FVector2D(pos.X - cX, pos.Y - cY);
    FString coordsMouse = "VertMouse : " + FString::SanitizeFloat(vertMouse.X) + " " + FString::SanitizeFloat(vertMouse.Y);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, coordsMouse);

    float Ang1 = FMath::Atan2(vert.X, vert.Y);
    float Ang2 = FMath::Atan2(vertMouse.X, vertMouse.Y);
    Ang = FMath::RadiansToDegrees(Ang1 - Ang2);
    if (Ang > 180.0f) Ang -= 360.0f; else if (Ang < -180.0f) Ang += 360.0f;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(Ang));

    return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FSlateBrush USelectionWheelUserWidget::UpdateImage(float Angle)
{
    selection_wheel_circle = FindObject<UTexture2D>(GetWorld(), TEXT("/Game/Assets/SelectionWheel/SelectionWheel.SelectionWheel"));
    selection_wheel_cross = FindObject<UTexture2D>(GetWorld(), TEXT("/Game/Assets/SelectionWheel/SelectionWheel.SelectionWheel"));
    selection_wheel_question = FindObject<UTexture2D>(GetWorld(), TEXT("/Game/Assets/SelectionWheel/SelectionWheel.SelectionWheel"));
    selection_wheel_left = FindObject<UTexture2D>(GetWorld(), TEXT("/Game/Assets/SelectionWheel/SelectionWheel.SelectionWheel"));
    selection_wheel_right = FindObject<UTexture2D>(GetWorld(), TEXT("/Game/Assets/SelectionWheel/SelectionWheel.SelectionWheel"));
    selection_wheel_straight = FindObject<UTexture2D>(GetWorld(), TEXT("/Game/Assets/SelectionWheel/SelectionWheel.SelectionWheel"));
    
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::SanitizeFloat(Ang));
    if (Ang < 30.f && Ang > - 30.f)
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(selection_wheel_circle, 100, 100);
    else if (Ang > 30.f && Ang < 90.f)
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(selection_wheel_cross, 100, 100);
    else if (Ang > 90.f && Ang < 150.f)
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(selection_wheel_question, 100, 100);
    else if (Ang > 150.f && Ang < -150.f)
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(selection_wheel_left, 100, 100);
    else if (Ang < -90.f && Ang > -150.f)
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(selection_wheel_right, 100, 100);
    else //if (Ang < -30.f && Ang > -90.f)
        return UWidgetBlueprintLibrary::MakeBrushFromTexture(selection_wheel_straight, 100, 100);
}

float USelectionWheelUserWidget::GetAngle() {
    return Ang;
}