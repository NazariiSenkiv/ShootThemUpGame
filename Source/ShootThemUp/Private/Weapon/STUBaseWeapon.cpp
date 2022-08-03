// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    SetRootComponent(WeaponMeshComponent);
}

void ASTUBaseWeapon::StartFire()
{
}

void ASTUBaseWeapon::StopFire()
{
}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    checkf(DefaultAmmo.BulletsCount > 0, TEXT("Bullets count couldn't be <= 0"));
    checkf(DefaultAmmo.ClipsCount >= 0, TEXT("Clips count couldn't be < 0"));
    
    CurrentAmmo = DefaultAmmo;
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    ACharacter* Player = Cast<ACharacter>(GetOwner());
    if (!Player)
        return nullptr;

    return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto* PlayerController = GetPlayerController();
    if (!PlayerController)
        return false;

    PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    TraceStart = ViewLocation;
    TraceEnd = ViewLocation + ViewRotation.Vector() * TraceLength;
    return true;
}

FVector ASTUBaseWeapon::GetMuzzleLocation() const
{
    return WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);
}

FVector ASTUBaseWeapon::GetMuzzleDirectionVector() const
{
    return WeaponMeshComponent->GetSocketRotation(MuzzleSocketName).Vector();
}

bool ASTUBaseWeapon::FindLineTraceHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    UWorld* World = GetWorld();
    if (!World)
        return false;

    World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
    return true;
}

void ASTUBaseWeapon::MakeShot()
{
}

void ASTUBaseWeapon::CauseDamage(const FHitResult& HitResult)
{
    const AActor* Actor = HitResult.GetActor();
    if (!Actor)
        return;

    HitResult.GetActor()->TakeDamage(BaseDamage, FDamageEvent(),
        GetPlayerController(), this);
}

bool ASTUBaseWeapon::TraceShoot(FVector& ShootStart, FVector& ShootEnd, FHitResult& Hit)
{
    UWorld* World = GetWorld();
    if (!World)
        return false;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
        return false;

    ShootStart = GetMuzzleLocation();
    const FVector MuzzleDirection = GetMuzzleDirectionVector();

    if (!FindLineTraceHit(Hit, TraceStart, TraceEnd))
        return false;

    const FVector ShootDirection = Hit.bBlockingHit
                                       ? (Hit.ImpactPoint - ShootStart)
                                       : (TraceEnd - ShootStart);

    const float VectorProjection = FVector::DotProduct(MuzzleDirection, ShootDirection.GetSafeNormal());
    float BulletDeviationAngle = FMath::RadiansToDegrees(FMath::Acos(VectorProjection));

    if (BulletDeviationAngle <= MaxBulletDeviationAngle)
    {
        ShootEnd = ShootStart + ShootDirection;
    }
    else
    {
        ShootEnd = TraceEnd;
        Hit = FHitResult();
    }

    return true;
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.IsAmmoInfinite && CurrentAmmo.ClipsCount == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.BulletsCount == 0;
}

void ASTUBaseWeapon::ChangeClip()
{
    if (!CurrentAmmo.IsAmmoInfinite)
    {
        if (CurrentAmmo.ClipsCount == 0)
        {
            UE_LOG(LogBaseWeapon, Warning, TEXT("No Clips?"));
            return;
        }
        
        --CurrentAmmo.ClipsCount;
    }

    CurrentAmmo.BulletsCount = DefaultAmmo.BulletsCount;

    UE_LOG(LogBaseWeapon, Display, TEXT("=== Clip changed ==="));
}

bool ASTUBaseWeapon::CanReload() const
{
    return CurrentAmmo.BulletsCount < DefaultAmmo.BulletsCount && CurrentAmmo.ClipsCount > 0;
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    if (CurrentAmmo.BulletsCount == 0)
    {
        UE_LOG(LogBaseWeapon, Warning, TEXT("No Bullets?"));
        return;
    }
    
    CurrentAmmo.BulletsCount--;
    LogAmmo();

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        
        OnClipEmpty.Broadcast();
    }
}

void ASTUBaseWeapon::LogAmmo() const
{
    FString AmmoInfo = FString::Printf(TEXT("Ammo: %d / %s"), CurrentAmmo.BulletsCount,
        CurrentAmmo.IsAmmoInfinite ? TEXT("Inf") : *FString::FromInt(CurrentAmmo.ClipsCount));

    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}
