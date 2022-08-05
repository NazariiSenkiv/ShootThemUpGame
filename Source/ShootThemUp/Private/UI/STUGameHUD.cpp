// Shoot Them Up Game, All Rights Reserved


#include "UI/STUGameHUD.h"

#include "Blueprint/UserWidget.h"

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    const auto Widget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);

    if (!Widget)
        return;

    Widget->AddToViewport();
}
