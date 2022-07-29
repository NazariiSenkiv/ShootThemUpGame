// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "STUBaseCharacter.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;
class USceneComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    USTUWeaponComponent();

    void StartFire();
    void StopFire();

    void NextWeapon();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<TSubclassOf<ASTUBaseWeapon>> WeaponClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName ArmorySocketName = "WeaponArmorySocket";

    // Called when the game starts
    virtual void BeginPlay() override;

    void SpawnWeapons();

private:
    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    int32 CurrentWeaponId = 0;

    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* Component, const FName& SocketName);
    void EquipWeapon(int32 WeaponId);
};
