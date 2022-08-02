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
    if (!World || IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    FVector ShootStart, ShootEnd;
    FHitResult Hit;
    if (!TraceShoot(ShootStart, ShootEnd, Hit))
        return;

    FVector ShootDirection = (ShootEnd - ShootStart).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleLocation());
    ASTUProjectile* Projectile = World->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);

    if (!Projectile)
        return;

    Projectile->SetMovementDirection(ShootDirection);
    Projectile->SetOwner(GetOwner());
    Projectile->FinishSpawning(SpawnTransform);

    DecreaseAmmo();
}
