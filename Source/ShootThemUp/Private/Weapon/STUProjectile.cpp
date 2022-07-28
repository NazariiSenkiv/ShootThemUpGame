// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STUProjectile.h"

#include "Components/SphereComponent.h"

ASTUProjectile::ASTUProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

    SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollision");
    SphereCollisionComponent->InitSphereRadius(5.0f);
    SetRootComponent(SphereCollisionComponent);
}

void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

