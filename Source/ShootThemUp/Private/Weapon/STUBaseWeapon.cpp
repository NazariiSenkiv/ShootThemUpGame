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

void ASTUBaseWeapon::MakeShot()
{
    UWorld* World = GetWorld();
    if (!World)
        return;

    ACharacter* Player = Cast<ACharacter>(GetOwner());
    if (!Player)
        return;

    APlayerController* PlayerController = Player->GetController<APlayerController>();
    if (!PlayerController)
        return;

    FVector ViewLocation;
    FRotator ViewRotation;
    PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

    const FTransform SocketTransform = WeaponMeshComponent->GetSocketTransform(MuzzleSocketName);
    const FVector MuzzleLocation = SocketTransform.GetLocation();
    const FVector MuzzleDirection = SocketTransform.GetRotation().GetForwardVector();
    const FVector TraceEnd = ViewLocation + ViewRotation.Vector() * TraceLength;
    
    FHitResult HitResult;
    World->LineTraceSingleByChannel(HitResult, ViewLocation, TraceEnd, ECollisionChannel::ECC_Visibility);
    
    const FVector ShootDirection = HitResult.bBlockingHit ?
        (HitResult.ImpactPoint - MuzzleLocation).GetSafeNormal()
            : (TraceEnd - MuzzleLocation).GetSafeNormal();

    const float VectorProjection = FVector::DotProduct(MuzzleDirection, ShootDirection);
    
    if (HitResult.bBlockingHit && VectorProjection > 0.0)
    {
        DrawDebugLine(World, MuzzleLocation, HitResult.ImpactPoint, FColor::Red,
            false, 5.0f, 0, 3.0f);
        DrawDebugSphere(World, HitResult.ImpactPoint, 10.0f, 24, FColor::Yellow,
            false, 5.0f, 0, 3.0f);
    }
    else
    {
        DrawDebugLine(World, MuzzleLocation, TraceEnd, FColor::Red,
            false, 5.0f, 0, 3.0f);
    }
}
