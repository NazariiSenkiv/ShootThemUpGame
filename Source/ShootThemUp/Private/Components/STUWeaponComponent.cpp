// Shoot Them Up Game, All Rights Reserved


#include "Components/STUWeaponComponent.h"

#include "GameFramework/Character.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::StartFire()
{
    if (!CanFire())
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
    if (!CanEquip())
        return;

    CurrentWeaponId = (CurrentWeaponId + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponId);
}

void USTUWeaponComponent::Reload()
{
    if (!CanReload())
        return;

    IsReloadAnimInProgress = true;
    PlayCharacterAnimMontage(CurrentReloadAnimMontage);
}

// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapons();
    InitAnimationNotifies();
    EquipWeapon(CurrentWeaponId);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;

    for (auto* Weapon : Weapons)
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

    for (auto WeaponData : WeaponsData)
    {
        auto Weapon = World->SpawnActor<ASTUBaseWeapon>(WeaponData.WeaponClass);

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
    if (WeaponId < 0 || WeaponId >= Weapons.Num())
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("WeaponId out of range"));
        return;
    }

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    if (CurrentWeapon)
    {
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), ArmorySocketName);
        CurrentWeapon->StopFire();
    }

    CurrentWeapon = Weapons[WeaponId];
    const auto CurrentWeaponData = WeaponsData.FindByPredicate([&](const FWeaponData& Data)
    {
        return Data.WeaponClass == CurrentWeapon->GetClass();
    });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    IsEquipAnimInProgress = true;
    PlayCharacterAnimMontage(EquipAnimMontage);

    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponSocketName);
}

void USTUWeaponComponent::PlayCharacterAnimMontage(UAnimMontage* Anim)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    Character->PlayAnimMontage(Anim);
}

void USTUWeaponComponent::InitAnimationNotifies()
{
    const auto EquipFinishedNotify = FindFirstNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);

    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipAnimFinishedHandle);
    }

    for (FWeaponData& WeaponData : WeaponsData)
    {
        const auto ReloadFinishedNotify = FindFirstNotifyByClass<USTUReloadFinishedAnimNotify>(WeaponData.ReloadAnimMontage);

        if (!ReloadFinishedNotify)
            continue;

        ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadAnimFinishedHandle);
    }
}

void USTUWeaponComponent::OnEquipAnimFinishedHandle(USkeletalMeshComponent* SkeletalMesh)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !SkeletalMesh)
        return;

    if (Character->GetMesh() == SkeletalMesh)
    {
        IsEquipAnimInProgress = false;
    }
}

void USTUWeaponComponent::OnReloadAnimFinishedHandle(USkeletalMeshComponent* SkeletalMesh)
{
    const ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !SkeletalMesh)
        return;

    if (Character->GetMesh() == SkeletalMesh)
    {
        IsReloadAnimInProgress = false;
    }
}

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !IsEquipAnimInProgress && !IsReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !IsEquipAnimInProgress && !IsReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
    return CurrentWeapon && !IsEquipAnimInProgress && !IsReloadAnimInProgress;
}
