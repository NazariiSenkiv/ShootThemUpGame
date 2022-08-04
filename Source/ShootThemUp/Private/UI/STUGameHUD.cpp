// Shoot Them Up Game, All Rights Reserved


#include "UI/STUGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();

    DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    const auto Widget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);

    if (!Widget)
        return;

    Widget->AddToViewport();
}

void ASTUGameHUD::DrawCrossHair()
{
    FVector2D ScreenCenter(Canvas->SizeX / 2.0, Canvas->SizeY / 2.0);

    float LineHalfLength = 10.0;
    FLinearColor LineColor = FLinearColor::Green;
    float LineThickness = 1.0;
    
    float SquareHalfLength = LineHalfLength*FMath::Sqrt(2.0);
    
    DrawLine(ScreenCenter.X - SquareHalfLength, ScreenCenter.Y - SquareHalfLength,
        ScreenCenter.X + SquareHalfLength, ScreenCenter.Y + SquareHalfLength, LineColor,
        LineThickness);
    DrawLine(ScreenCenter.X - SquareHalfLength, ScreenCenter.Y + SquareHalfLength,
        ScreenCenter.X + SquareHalfLength, ScreenCenter.Y - SquareHalfLength, LineColor,
        LineThickness);
    DrawLine(ScreenCenter.X, ScreenCenter.Y,
        ScreenCenter.X, ScreenCenter.Y, FLinearColor::Red, 3.0);
}
