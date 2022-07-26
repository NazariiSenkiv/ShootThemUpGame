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
    if (!World)
        return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
        return;

    const FVector MuzzleLocation = GetMuzzleLocation();
    const FVector MuzzleDirection = GetMuzzleDirectionVector();

    FHitResult HitResult;
    if (!FindLineTraceHit(HitResult, TraceStart, TraceEnd))
        return;

    const FVector ShootDirection = HitResult.bBlockingHit
                                       ? (HitResult.ImpactPoint - MuzzleLocation).GetSafeNormal()
                                       : (TraceEnd - MuzzleLocation).GetSafeNormal();

    const float VectorProjection = FVector::DotProduct(MuzzleDirection, ShootDirection);

    if (HitResult.bBlockingHit && VectorProjection > 0.0)
    {
        DrawDebugLine(World, MuzzleLocation, HitResult.ImpactPoint, FColor::MakeRandomColor(),
            false, 5.0f, 0, 3.0f);
        DrawDebugSphere(World, HitResult.ImpactPoint, 10.0f, 24, FColor::Yellow,
            false, 5.0f, 0, 3.0f);

        CauseDamage(HitResult);
    }
    else
    {
        DrawDebugLine(World, MuzzleLocation, TraceEnd, FColor::MakeRandomColor(),
            false, 5.0f, 0, 3.0f);
    }
}