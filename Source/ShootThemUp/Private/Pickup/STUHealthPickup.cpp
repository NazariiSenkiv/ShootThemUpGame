// Shoot Them Up Game, All Rights Reserved


#include "Pickup/STUHealthPickup.h"

#include "STUHealthComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GiveTo(APawn* PlayerPawn)
{
    const auto HealthComponent = STUUtils::GetPlayerComponent<USTUHealthComponent>(PlayerPawn);
    if (!HealthComponent)
        return false;

    return HealthComponent->TryHeal(HealAmount);
}
