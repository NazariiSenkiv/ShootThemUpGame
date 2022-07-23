// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;
class ASTUBaseCharacter;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUBaseWeapon();

    virtual void Fire();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Mesh")
    USkeletalMeshComponent* WeaponMeshComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Shoot")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Shoot")
    float TraceLength = 2000.0f;
    
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    void MakeShot();
};
