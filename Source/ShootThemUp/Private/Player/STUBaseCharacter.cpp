// Shoot Them Up Game, All Rights Reserved


#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    CameraDistanceSensitivity = CameraMaxDistance / 2.0f;
    DefaultMaxSpeed = GetCharacterMovement()->MaxWalkSpeed; 
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnRight", this, &ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("MoveCameraAway", this, &ASTUBaseCharacter::MoveCameraAway);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::StartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::StopRunning);
}

bool ASTUBaseCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

void ASTUBaseCharacter::MoveForward(float Axis)
{
    IsMovingForward = Axis > 0.0f;
    AddMovementInput(GetActorForwardVector(), Axis);
}

void ASTUBaseCharacter::MoveRight(float Axis)
{
    AddMovementInput(GetActorRightVector(), Axis);
}

void ASTUBaseCharacter::MoveCameraAway(float Axis)
{
    float NewSpringArmLength = SpringArmComponent->TargetArmLength +
                               Axis * CameraDistanceSensitivity * GetWorld()->DeltaTimeSeconds;

    if (NewSpringArmLength < CameraMaxDistance && NewSpringArmLength > CameraMinDistance)
    {
        SpringArmComponent->TargetArmLength = NewSpringArmLength;
    }
}

void ASTUBaseCharacter::StartRunning()
{
    WantsToRun = true;
    if (IsRunning())
    {
        GetCharacterMovement()->MaxWalkSpeed = MaxRunningSpeed;
    }
}

void ASTUBaseCharacter::StopRunning()
{
    WantsToRun = false;
    GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;
}
