// Shoot Them Up Game, All Rights Reserved


#include "Components/STUWeaponComponent.h"

#include "GameFramework/Character.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"

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
    PlayCharacterAnimMontage(CurrentReloadAnimMontage);
}

// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapons();
    InitAnimations();
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

void USTUWeaponComponent::InitAnimations()
{
    if (!EquipAnimMontage)
        return;

    const auto AnimNotifyEvents = EquipAnimMontage->Notifies;

    for (auto& AnimNotifyEvent : AnimNotifyEvents)
    {
        auto AnimNotify = Cast<USTUEquipFinishedAnimNotify>(AnimNotifyEvent.Notify);

        if (AnimNotify)
        {
            AnimNotify->OnEquipAnimFinished.AddUObject(this, &USTUWeaponComponent::OnEquipAnimFinishedHandle);
            break;
        }
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

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !IsEquipAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !IsEquipAnimInProgress;
}
