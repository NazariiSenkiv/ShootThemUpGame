// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "STUBaseCharacter.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;
class USceneComponent;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UAnimMontage* ReloadAnimMontage;
};

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

    void Reload();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponsData;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName ArmorySocketName = "WeaponArmorySocket";

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* EquipAnimMontage;

    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void SpawnWeapons();

    template <typename T>
    T* FindFirstNotifyByClass(UAnimSequenceBase* Anim)
    {
        if (!Anim)
            return nullptr;

        const auto AnimNotifyEvents = Anim->Notifies;

        for (auto& AnimNotifyEvent : AnimNotifyEvents)
        {
            auto AnimNotify = Cast<T>(AnimNotifyEvent.Notify);

            if (AnimNotify)
            {
                return AnimNotify;
            }
        }
        return nullptr;
    }
    
private:
    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;
    
    int32 CurrentWeaponId = 0;

    bool IsEquipAnimInProgress = false;
    bool IsReloadAnimInProgress = false;

    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* Component, const FName& SocketName);
    void EquipWeapon(int32 WeaponId);

    void PlayCharacterAnimMontage(UAnimMontage* Anim);

    void InitAnimationNotifies();
    
    void OnEquipAnimFinishedHandle(USkeletalMeshComponent* SkeletalMesh);
    void OnReloadAnimFinishedHandle(USkeletalMeshComponent* SkeletalMesh);

    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;
};
