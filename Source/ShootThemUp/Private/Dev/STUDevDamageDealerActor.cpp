// Shoot Them Up Game, All Rights Reserved


#include "Dev/STUDevDamageDealerActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASTUDevDamageDealerActor::ASTUDevDamageDealerActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
    SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void ASTUDevDamageDealerActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ASTUDevDamageDealerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 30, Color);

    UGameplayStatics::ApplyRadialDamage(GetWorld(), DamagePerSec * DeltaTime, GetActorLocation(), Radius,
        nullptr, {}, this, nullptr, bDoFullDamage);
}
