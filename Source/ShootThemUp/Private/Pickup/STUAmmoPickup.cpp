// Shoot Them Up Game, All Rights Reserved


#include "Pickup/STUAmmoPickup.h"

#include "STUUtils.h"
#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::GiveTo(APawn* PlayerPawn)
{
    const auto HealthComponent = STUUtils::GetPlayerComponent<USTUHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    const auto WeaponComponent = STUUtils::GetPlayerComponent<USTUWeaponComponent>(PlayerPawn);
    if (!WeaponComponent)
        return false;
    
    return WeaponComponent->TryAddAmmo(WeaponClass, ClipsCount);
}