// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
public:
    ASTURifleWeapon();
    
    virtual void StartFire() override;
    virtual void StopFire() override;
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
    float ShotDelay = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot", meta = (ClampMin = "0.0", ClampMax = "30.0"))
    float DeflectionHalfAngle = 1.4f;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* FXComponent;

    virtual void BeginPlay() override;
    
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

    virtual void MakeShot() override;
private:
    FTimerHandle ShootTimerHandle;
};
