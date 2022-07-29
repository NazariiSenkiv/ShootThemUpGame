// Shoot Them Up Game, All Rights Reserved


#include "Player/STUBaseCharacter.h"

#include "STUWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUCharacter, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0, 85, 80);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);

    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChangedHandle);

    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);

    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeathHandle);

    CameraDistanceSensitivity = CameraMaxDistance / 2.0f;
    DefaultMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnLandedHandle);
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
    check(PlayerInputComponent);
    check(WeaponComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnRight", this, &ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("MoveCameraAway", this, &ASTUBaseCharacter::MoveCameraAway);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::StartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::StopRunning);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);
}

bool ASTUBaseCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

float ASTUBaseCharacter::GetMovementAngle() const
{
    if (GetVelocity().IsZero())
        return 0.0f;

    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleRad = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto AngleDeg = FMath::RadiansToDegrees(AngleRad);
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

    return CrossProduct.IsZero() ? AngleDeg : AngleDeg * FMath::Sign(CrossProduct.Z);
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

void ASTUBaseCharacter::OnDeathHandle()
{
    UE_LOG(LogSTUCharacter, Display, TEXT("Dead"));

    PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();
    bUseControllerRotationYaw = false;

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    WeaponComponent->StopFire();
    SetLifeSpan(LifeSpanOnDeath);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUBaseCharacter::OnHealthChangedHandle(float Health)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::OnLandedHandle(const FHitResult& Hit)
{
    const float FallVelocity = -GetVelocity().Z;

    if (FallVelocity < FallDamageVelocityRange.X)
        return;

    const float FallDamage = FMath::GetMappedRangeValueClamped(FallDamageVelocityRange,
        FallDamageRange, FallVelocity);

    TakeDamage(FallDamage, FDamageEvent{}, nullptr, nullptr);
}
