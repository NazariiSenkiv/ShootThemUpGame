// Shoot Them Up Game, All Rights Reserved


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{    
    const auto Player = GetOwningPlayerPawn();

    if (!Player)
        return 0.0f;

    const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
    const auto HealthComponent = Cast<USTUHealthComponent>(Component);

    if (!HealthComponent)
        return 0.0f;
    
    return  HealthComponent->GetHealthPercent();
}

FLinearColor USTUPlayerHUDWidget::GetHealthBarColor() const
{
    float HealthPercent = GetHealthPercent();

    return FLinearColor(1-HealthPercent, HealthPercent, 0);
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto PlayerPawn = GetOwningPlayerPawn();
    if (!PlayerPawn)
        return false;

    const auto Controller = PlayerPawn->GetComponentByClass(USTUWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<USTUWeaponComponent>(Controller);
    if (!WeaponComponent)
        return false;
    
    return WeaponComponent->GetCurrentWeaponUIData(UIData);
}
