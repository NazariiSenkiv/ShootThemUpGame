// Shoot Them Up Game, All Rights Reserved


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Dev/ExplosionDamageType.h"
#include "Dev/HitDamageType.h"

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

    Health = MaxHealth;

    AActor* Owner = GetOwner();
    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::TakeAnyDamageHandle);
    }
}

void USTUHealthComponent::TakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
    AController* InstigatedBy, AActor* DamageCauser)
{
    Health -= Damage;

    if (DamageType)
    {
        if (DamageType->IsA<UHitDamageType>())
        {
            UE_LOG(HealthComponentLog, Display, TEXT("Hit"));
        }
        else if (DamageType->IsA<UExplosionDamageType>())
        {
             UE_LOG(HealthComponentLog, Display, TEXT("BANG"));
        }
    }
}
