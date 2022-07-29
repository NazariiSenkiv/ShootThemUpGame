// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STULauncherWeapon.h"
#include "STUProjectile.h"
#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    UWorld* World = GetWorld();
    if (!World)
        return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
        return;

    FHitResult HitResult;
    if (!FindLineTraceHit(HitResult, TraceStart, TraceEnd))
        return;

    const FVector MuzzleLocation = GetMuzzleLocation();
    const FVector ShootDirection = HitResult.bBlockingHit
                                       ? (HitResult.ImpactPoint - MuzzleLocation).GetSafeNormal()
                                       : (TraceEnd - MuzzleLocation).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleLocation());
    ASTUProjectile* Projectile = World->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);

    if (!Projectile)
        return;

    Projectile->SetMovementDirection(ShootDirection);
    Projectile->SetOwner(GetOwner());
    Projectile->FinishSpawning(SpawnTransform);
}
