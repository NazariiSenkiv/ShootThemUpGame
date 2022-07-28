// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STUProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASTUProjectile::ASTUProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

    SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollision");
    SphereCollisionComponent->InitSphereRadius(5.0f);
    SetRootComponent(SphereCollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;
}

void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();

    check(MovementComponent);
    
    MovementComponent->Velocity = MovementComponent->InitialSpeed * MovementDirection;
    SetLifeSpan(5.0f);
}

