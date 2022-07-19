// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUDevDamageDealerActor.generated.h"


UCLASS()
class SHOOTTHEMUP_API ASTUDevDamageDealerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUDevDamageDealerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* SceneComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Radius = 299.99f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor Color = FColor::Yellow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamagePerSec = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDoFullDamage = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UDamageType> DamageType;
    
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
