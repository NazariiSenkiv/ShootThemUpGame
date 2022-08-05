// Shoot Them Up Game, All Rights Reserved


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = GetOwningPlayerComponent<USTUHealthComponent>();
    if (!HealthComponent)
        return 0.0f;

    return HealthComponent->GetHealthPercent();
}

FLinearColor USTUPlayerHUDWidget::GetHealthBarColor() const
{
    float HealthPercent = GetHealthPercent();

    return FLinearColor(1 - HealthPercent, HealthPercent, 0);
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = GetOwningPlayerComponent<USTUWeaponComponent>();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = GetOwningPlayerComponent<USTUWeaponComponent>();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetCurrentAmmoData(AmmoData);
}
