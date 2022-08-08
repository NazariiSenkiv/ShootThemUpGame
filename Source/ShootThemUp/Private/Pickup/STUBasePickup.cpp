// Shoot Them Up Game, All Rights Reserved


#include "Pickup/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

// Sets default values
ASTUBasePickup::ASTUBasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    UE_LOG(LogBasePickup, Display, TEXT("Picked by %s"), *(OtherActor->GetName()));

    Destroy();
}

// Called every frame
void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

