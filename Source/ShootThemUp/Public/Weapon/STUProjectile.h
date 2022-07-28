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
    
    virtual void BeginPlay() override;

private:
    FVector MovementDirection;
};
