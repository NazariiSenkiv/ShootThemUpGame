// Shoot Them Up Game, All Rights Reserved


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
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

void ASTUBaseWeapon::Fire()
{
    UE_LOG(LogBaseWeapon, Display, TEXT("Piv-piv"));

    MakeShot();
}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
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

void ASTUBaseWeapon::CauseDamage(const FHitResult& HitResult)
{
    const AActor* Actor = HitResult.GetActor();
    if (!Actor)
        return;
    
    HitResult.GetActor()->TakeDamage(BulletDamage, FDamageEvent(),
            GetPlayerController(), this);
}
