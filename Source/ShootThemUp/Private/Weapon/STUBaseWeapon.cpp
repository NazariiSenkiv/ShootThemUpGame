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
