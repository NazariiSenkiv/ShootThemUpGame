// Shoot Them Up Game, All Rights Reserved


#include "Pickup/STUAmmoPickup.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::GiveTo(APawn* PlayerPawn)
{
    UE_LOG(LogAmmoPickup, Display, TEXT("Ammo pickup was taken by %s"), *PlayerPawn->GetName());
    return true;
}