// Shoot Them Up Game, All Rights Reserved


#include "Components/STUWeaponComponent.h"

#include "GameFramework/Character.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::StartFire()
{
    if (!CurrentWeapon)
        return;
    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
        return;
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
    CurrentWeaponId = (CurrentWeaponId + 1)%Weapons.Num();
    EquipWeapon(CurrentWeaponId);
}

// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapons();
    EquipWeapon(CurrentWeaponId);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;

    for (auto* Weapon: Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::SpawnWeapons()
{
    UWorld* World = GetWorld();
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!World || !Character)
        return;

    for (auto WeaponClass: WeaponClasses)
    {
        auto Weapon = World->SpawnActor<ASTUBaseWeapon>(WeaponClass);

        if (!Weapon)
            continue;
        
        Weapons.Add(Weapon);
        Weapon->SetOwner(Character);
        AttachWeaponToSocket(Weapon, Character->GetMesh(), ArmorySocketName);
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* Component, const FName& SocketName)
{
    if (!Weapon || !Component)
        return;
    
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(Component, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponId)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;
    
    WeaponId = FMath::Clamp(WeaponId, 0, Weapons.Num()-1);
    
    if (CurrentWeapon)
    {
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), ArmorySocketName);
        CurrentWeapon->StopFire();
    }
    
    CurrentWeapon = Weapons[WeaponId];
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponSocketName);
}
