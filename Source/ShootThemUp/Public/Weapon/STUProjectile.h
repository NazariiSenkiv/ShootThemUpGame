// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "STUProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
    GENERATED_BODY()

public:
    ASTUProjectile();

    void SetMovementDirection(const FVector& Direction) { MovementDirection = Direction; }

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
    USphereComponent* SphereCollisionComponent;

    UPROPERTY(VisibleDefaultsOnly, Category = "Movement")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float ExplosionRadius = 200.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float ExplosionDamage = 50.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    bool DoFullDamage = false;

    UPROPERTY(EditDefaultsOnly, Category = "Shoot")
    float LifeSeconds = 5.0f;

    virtual void BeginPlay() override;

    AController* GetController() const;
    
private:
    FVector MovementDirection;

    UFUNCTION()
    void OnComponentHitHandle(UPrimitiveComponent* HitedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
