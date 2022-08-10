// Shoot Them Up Game, All Rights Reserved


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All)

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);
    
    SetHealth(MaxHealth);

    AActor* Owner = GetOwner();
    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::TakeAnyDamageHandle);
    }
}

void USTUHealthComponent::Heal(float HealAmount)
{
    SetHealth(Health + HealAmount);
}

bool USTUHealthComponent::TryHeal(float HealAmount)
{
    if (HealAmount < 0 || IsHealthFull() || IsDead())
        return false;

    Heal(HealAmount);
    return true;
}

void USTUHealthComponent::AutoHeal()
{
    if (bAutoHealEnabled)
    {
        if (Health < MaxHealth)
        {
            Heal(AutoHealAmount);
        }
        else
        {
            StopAutoHeal();
        }
    }
}

void USTUHealthComponent::StartAutoHeal()
{
    GetWorld()->GetTimerManager().SetTimer(AutoHealTimerHandle, this, &USTUHealthComponent::AutoHeal,
        HealUpdateTime, true, HealDelay);
}

void USTUHealthComponent::StopAutoHeal()
{
    if (AutoHealTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
    }
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}

void USTUHealthComponent::TakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
    AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead())
        return;

    SetHealth(Health - Damage);

    // stop previous timer
    StopAutoHeal();

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (bAutoHealEnabled)
    {
        StartAutoHeal();
    }
}
