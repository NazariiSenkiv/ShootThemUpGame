// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;
class ASTUBaseCharacter;

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 BulletsCount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 ClipsCount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool IsAmmoInfinite = false;
};

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASTUBaseWeapon();

    virtual void StartFire();
    virtual void StopFire();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Mesh")
    USkeletalMeshComponent* WeaponMeshComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Shoot")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Shoot")
    float TraceLength = 2000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float BaseDamage = 10.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Shoot")
    float MaxBulletDeviationAngle = 30.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FAmmoData DefaultAmmo {15, 10, false};
    
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    APlayerController* GetPlayerController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    FVector GetMuzzleLocation() const;
    FVector GetMuzzleDirectionVector() const;
    bool FindLineTraceHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    virtual void MakeShot();
    void CauseDamage(const FHitResult& HitResult);

    bool TraceShoot(FVector& ShootStart, FVector& ShootEnd, FHitResult& Hit);

    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;

    void ChangeClip();
    void DecreaseAmmo();

    void LogAmmo() const;
    
private:
    FAmmoData CurrentAmmo;
};
