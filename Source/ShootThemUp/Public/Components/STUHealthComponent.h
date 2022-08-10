// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    USTUHealthComponent();

    FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() { return FMath::IsNearlyZero(Health); }

    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const { return Health / MaxHealth; }

    void Heal(float HealAmount);
    bool TryHeal(float HealAmount);

    bool IsHealthFull() const { return FMath::IsNearlyEqual(Health, MaxHealth); }
protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin="0.0", ClampMax="1000.0"))
    float MaxHealth = 100.0f;

    // Auto heal
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool bAutoHealEnabled = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "bAutoHealEnabled"))
    float HealUpdateTime = 0.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "bAutoHealEnabled"))
    float HealDelay = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "bAutoHealEnabled"))
    float AutoHealAmount = 1.0f;

private:
    FTimerHandle AutoHealTimerHandle;

    float Health = 0.0f;

    void AutoHeal();
    void StartAutoHeal();
    void StopAutoHeal();

    void SetHealth(float NewHealth);

    UFUNCTION()
    void TakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy,
        AActor* DamageCauser);
};
