// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickup/STUBasePickup.h"
#include "STUAmmoPickup.generated.h"

class ASTUBaseWeapon;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUAmmoPickup : public ASTUBasePickup
{
	GENERATED_BODY()

private:
    virtual bool GiveTo(APawn* PlayerPawn) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1"))
    int32 ClipsCount = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;
};
