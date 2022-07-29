// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollision");
    SphereCollisionComponent->InitSphereRadius(5.0f);
    SphereCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SetRootComponent(SphereCollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();

    check(MovementComponent);
    check(SphereCollisionComponent);

    MovementComponent->Velocity = MovementComponent->InitialSpeed * MovementDirection;

    SphereCollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnComponentHitHandle);
    SphereCollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    
    SetLifeSpan(LifeSeconds);
}

AController* ASTUProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}

void ASTUProjectile::OnComponentHitHandle(UPrimitiveComponent* HitedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
    FVector NormalImpulse, const FHitResult& Hit)
{
    UWorld* World = GetWorld();
    if (!World)
        return;

    MovementComponent->StopMovementImmediately();
    
    UGameplayStatics::ApplyRadialDamage(World,
        ExplosionDamage,
        GetActorLocation(),
        ExplosionRadius,
        UDamageType::StaticClass(),
        {GetOwner()},
        this,
        GetController(),
        DoFullDamage);

    DrawDebugSphere(World, GetActorLocation(), ExplosionRadius, 24,
        FColor::MakeRandomColor(), false, 5.0f);

    Destroy();
}
