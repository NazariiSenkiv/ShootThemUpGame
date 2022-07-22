// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class UAnimMontage;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASTUBaseCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
    USTUHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    USTUWeaponComponent* WeaponComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CameraMinDistance = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CameraMaxDistance = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CameraDistanceSensitivity;

    bool WantsToRun = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MaxRunningSpeed = 1000.0f;

    float DefaultMaxSpeed;
    bool IsMovingForward = false;

    UPROPERTY(EditDefaultsOnly, Category = "Death")
    float LifeSpanOnDeath = 5.0f;
    
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D FallDamageVelocityRange{900.0, 1800.0};

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D FallDamageRange{10.0, 100.0};

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;
    
public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementAngle() const;

private:
    void MoveForward(float Axis);
    void MoveRight(float Axis);

    void MoveCameraAway(float Axis);

    void StartRunning();
    void StopRunning();

    void OnDeathHandle();

    void OnHealthChangedHandle(float Health);

    UFUNCTION()
    void OnLandedHandle(const FHitResult& Hit);
};
