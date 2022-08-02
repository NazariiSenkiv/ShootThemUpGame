// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"


void ASTURifleWeapon::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ASTURifleWeapon::MakeShot, ShotDelay, true);
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    TraceStart = ViewLocation;

    const float DeflectionHalfRad = FMath::DegreesToRadians(DeflectionHalfAngle);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), DeflectionHalfRad);
    TraceEnd = ViewLocation + ShootDirection * TraceLength;
    return true;
}

void ASTURifleWeapon::MakeShot()
{
    UWorld* World = GetWorld();
    if (!World || IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    UE_LOG(LogTemp, Display, TEXT("Make Shot"));
    
    FVector ShootStart, ShootEnd;
    FHitResult Hit;
    if (!TraceShoot(ShootStart, ShootEnd, Hit))
    {
        return;
    }

    DrawDebugLine(World, ShootStart, ShootEnd, FColor::MakeRandomColor(),
        false, 5.0f, 0, 3.0f);
    if (Hit.bBlockingHit)
    {
        DrawDebugSphere(World, ShootEnd, 10.0f, 24, FColor::Yellow,
            false, 5.0f, 0, 3.0f);

        CauseDamage(Hit);
    }

    DecreaseAmmo();
}
