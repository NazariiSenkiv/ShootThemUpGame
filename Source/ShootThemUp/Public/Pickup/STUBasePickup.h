// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;
class APawn;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUBasePickup();

protected:

    UPROPERTY(EditDefaultsOnly, Category = "Collision")
    USphereComponent* SphereCollisionComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup")
    float RespawnTime = 5.0f;
    
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
    virtual bool GiveTo(APawn* PlayerPawn);
    
    void PickupWasTaken();
    void Respawn();
};
