// Shoot Them Up Game, All Rights Reserved


#include "Pickup/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

// Sets default values
ASTUBasePickup::ASTUBasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
    SphereCollisionComponent->InitSphereRadius(50.0f);
    SphereCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(SphereCollisionComponent);
}

// Called when the game starts or when spawned
void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();

    check(SphereCollisionComponent);
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    APawn* OtherPawn = Cast<APawn>(OtherActor);
    
    if (GiveTo(OtherPawn))
    {
        PickupWasTaken();
    }
}

bool ASTUBasePickup::GiveTo(APawn* PlayerPawn)
{
    return false;
}

void ASTUBasePickup::PickupWasTaken()
{
    SphereCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    if (GetRootComponent())
    {
        GetRootComponent()->SetVisibility(false, true);
    }

    FTimerHandle RespawnTimerHandle;
    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
}

void ASTUBasePickup::Respawn()
{
    SphereCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    if (GetRootComponent())
    {
        GetRootComponent()->SetVisibility(true, true);
    }
}

