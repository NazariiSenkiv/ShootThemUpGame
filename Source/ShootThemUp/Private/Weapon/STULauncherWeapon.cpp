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

    FVector ShootStart, ShootEnd;
    FHitResult Hit;
    TraceShoot(ShootStart, ShootEnd, Hit);

    FVector ShootDirection = (ShootEnd - ShootStart).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleLocation());
    ASTUProjectile* Projectile = World->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);

    if (!Projectile)
        return;

    Projectile->SetMovementDirection(ShootDirection);
    Projectile->SetOwner(GetOwner());
    Projectile->FinishSpawning(SpawnTransform);
}
