// Shoot Them Up Game, All Rights Reserved


#include "Pickup/STUHealthPickup.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GiveTo(APawn* PlayerPawn)
{
    UE_LOG(LogHealthPickup, Display, TEXT("Health pickup was taken by %s"), *PlayerPawn->GetName());
    return true;
}
