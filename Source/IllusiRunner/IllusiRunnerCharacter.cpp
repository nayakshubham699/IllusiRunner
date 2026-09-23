#include "IllusiRunnerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "IllusiRunner.h"
#include "IllusionMovementComponent.h"

AIllusiRunnerCharacter::AIllusiRunnerCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate =
        FRotator(0.0f, 500.0f, 0.0f);

    GetCharacterMovement()->JumpZVelocity = 500.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    CameraBoom =
        CreateDefaultSubobject<USpringArmComponent>(
            TEXT("CameraBoom")
        );

    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera =
        CreateDefaultSubobject<UCameraComponent>(
            TEXT("FollowCamera")
        );

    FollowCamera->SetupAttachment(
        CameraBoom,
        USpringArmComponent::SocketName
    );

    FollowCamera->bUsePawnControlRotation = false;
}

void AIllusiRunnerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AIllusiRunnerCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent =
        Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(
            JumpAction,
            ETriggerEvent::Started,
            this,
            &AIllusiRunnerCharacter::DoJumpStart
        );

        EnhancedInputComponent->BindAction(
            JumpAction,
            ETriggerEvent::Completed,
            this,
            &AIllusiRunnerCharacter::DoJumpEnd
        );

        EnhancedInputComponent->BindAction(
            MoveAction,
            ETriggerEvent::Triggered,
            this,
            &AIllusiRunnerCharacter::Move
        );

        EnhancedInputComponent->BindAction(
            MouseLookAction,
            ETriggerEvent::Triggered,
            this,
            &AIllusiRunnerCharacter::Look
        );

        EnhancedInputComponent->BindAction(
            LookAction,
            ETriggerEvent::Triggered,
            this,
            &AIllusiRunnerCharacter::Look
        );
    }
    else
    {
        UE_LOG(
            LogIllusiRunner,
            Error,
            TEXT("'%s' Failed to find an Enhanced Input component!"),
            *GetNameSafe(this)
        );
    }
}

void AIllusiRunnerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AIllusiRunnerCharacter::Move(
    const FInputActionValue& Value)
{
    const FVector2D MovementVector =
        Value.Get<FVector2D>();

    UIllusionMovementComponent* Component =
        FindComponentByClass<UIllusionMovementComponent>();

    if (Component)
    {
        Component->Move(MovementVector);
    }
    else
    {
        DoMove(
            MovementVector.X,
            MovementVector.Y
        );
    }
}

void AIllusiRunnerCharacter::Look(
    const FInputActionValue& Value)
{
    const FVector2D LookAxisVector =
        Value.Get<FVector2D>();

    DoLook(
        LookAxisVector.X,
        LookAxisVector.Y
    );
}

void AIllusiRunnerCharacter::DoMove(
    float Right,
    float Forward)
{
    if (GetController() != nullptr)
    {
        const FRotator Rotation =
            GetController()->GetControlRotation();

        const FRotator YawRotation(
            0,
            Rotation.Yaw,
            0
        );

        const FVector ForwardDirection =
            FRotationMatrix(YawRotation)
            .GetUnitAxis(EAxis::X);

        const FVector RightDirection =
            FRotationMatrix(YawRotation)
            .GetUnitAxis(EAxis::Y);

        AddMovementInput(
            ForwardDirection,
            Forward
        );

        AddMovementInput(
            RightDirection,
            Right
        );
    }
}

void AIllusiRunnerCharacter::DoLook(
    float Yaw,
    float Pitch)
{
    if (GetController() != nullptr)
    {
        AddControllerYawInput(Yaw);
        AddControllerPitchInput(Pitch);
    }
}

void AIllusiRunnerCharacter::DoJumpStart()
{
    Jump();
}

void AIllusiRunnerCharacter::DoJumpEnd()
{
    StopJumping();
}