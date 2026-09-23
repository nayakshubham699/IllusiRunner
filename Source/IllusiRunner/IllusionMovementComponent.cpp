#include "IllusionMovementComponent.h"

#include "IllusiRunnerPlayerCharacter.h"
#include "IllusiRunnerMimicCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UIllusionMovementComponent::UIllusionMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PostPhysics;

    Player = nullptr;
    Mimic = nullptr;
    PlayerMovement = nullptr;
    MimicMovement = nullptr;

    CurrentMovementInput = FVector2D::ZeroVector;

    bMovementInputActive = false;
    bMovementBlocked = false;
    bPlayerJumping = false;
}

void UIllusionMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    Player =
        Cast<AIllusiRunnerPlayerCharacter>(
            GetOwner()
        );

    if (!Player)
    {
        return;
    }

    PlayerMovement =
        Player->GetCharacterMovement();

    if (MimicClass)
    {
        Mimic =
            Cast<AIllusiRunnerMimicCharacter>(
                UGameplayStatics::GetActorOfClass(
                    GetWorld(),
                    MimicClass
                )
            );
    }

    if (Mimic)
    {
        MimicMovement =
            Mimic->GetCharacterMovement();

        LastMimicLocation =
            Mimic->GetActorLocation();
    }

    LastPlayerLocation =
        Player->GetActorLocation();
}

void UIllusionMovementComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(
        DeltaTime,
        TickType,
        ThisTickFunction
    );

    if (!Player || !Mimic)
    {
        return;
    }

    if (!PlayerMovement || !MimicMovement)
    {
        return;
    }

    UpdateHorizontalMovement();

    UpdateVerticalMovement();

    LastPlayerLocation =
        Player->GetActorLocation();

    LastMimicLocation =
        Mimic->GetActorLocation();
}

void UIllusionMovementComponent::Move(FVector2D MovementInput)
{
    if (!Player)
    {
        return;
    }

    CurrentMovementInput = MovementInput;

    if (MovementInput.IsNearlyZero())
    {
        bMovementBlocked = false;
        bMovementInputActive = false;
        return;
    }

    bMovementInputActive = true;

    if (bMovementBlocked)
    {
        return;
    }

    if (!Player->GetController())
    {
        return;
    }

    // Rest of your movement code...

    const FRotator Rotation =
        Player->GetControlRotation();

    const FRotator YawRotation(
        0.0f,
        Rotation.Yaw,
        0.0f
    );

    const FVector ForwardDirection =
        FRotationMatrix(YawRotation)
        .GetUnitAxis(EAxis::X);

    const FVector RightDirection =
        FRotationMatrix(YawRotation)
        .GetUnitAxis(EAxis::Y);

    const FVector PlayerDirection =
        ForwardDirection * MovementInput.Y +
        RightDirection * MovementInput.X;

    Player->AddMovementInput(PlayerDirection);

    if (Mimic)
    {
        FVector MimicDirection = PlayerDirection;

        MimicDirection.Y *= -1.0f;

        Mimic->AddMovementInput(MimicDirection);
    }
}

void UIllusionMovementComponent::JumpPressed()
{
    if (!Player || !Mimic)
    {
        return;
    }

    bPlayerJumping = true;

    Player->Jump();
    Mimic->Jump();
}

void UIllusionMovementComponent::JumpReleased()
{
    if (!Player || !Mimic)
    {
        return;
    }

    Player->StopJumping();
    Mimic->StopJumping();
}

void UIllusionMovementComponent::UpdateHorizontalMovement()
{
    if (!bMovementInputActive)
    {
        return;
    }

    const FVector PlayerVelocity =
        PlayerMovement->Velocity;

    const FVector MimicVelocity =
        MimicMovement->Velocity;

    const float PlayerSpeed =
        PlayerVelocity.Size2D();

    const float MimicSpeed =
        MimicVelocity.Size2D();

    const FVector PlayerDelta =
        Player->GetActorLocation() -
        LastPlayerLocation;

    const FVector MimicDelta =
        Mimic->GetActorLocation() -
        LastMimicLocation;

    const float PlayerMovementDistance =
        FVector2D(
            PlayerDelta.X,
            PlayerDelta.Y
        ).Size();

    const float MimicMovementDistance =
        FVector2D(
            MimicDelta.X,
            MimicDelta.Y
        ).Size();

    const bool bPlayerTryingToMove =
        PlayerSpeed > 10.0f ||
        PlayerMovementDistance > 0.5f;

    const bool bMimicTryingToMove =
        MimicSpeed > 10.0f ||
        MimicMovementDistance > 0.5f;

    const bool bPlayerStopped =
        PlayerMovementDistance < 0.5f;

    const bool bMimicStopped =
        MimicMovementDistance < 0.5f;

    if (bPlayerTryingToMove &&
        bPlayerStopped &&
        bMimicTryingToMove)
    {
        StopBothCharacters();
        bMovementBlocked = true;
    }

    if (bMimicTryingToMove &&
        bMimicStopped &&
        bPlayerTryingToMove)
    {
        StopBothCharacters();
        bMovementBlocked = true;
    }
}

void UIllusionMovementComponent::UpdateVerticalMovement()
{
    if (!Player || !Mimic)
    {
        return;
    }

    if (bPlayerJumping)
    {
        if (MimicMovement->IsMovingOnGround())
        {
            bPlayerJumping = false;
        }
        else
        {
            return;
        }
    }

    const float MimicZ =
        Mimic->GetActorLocation().Z;

    const float PlayerZ =
        Player->GetActorLocation().Z;

    const float ZDifference =
        MimicZ - PlayerZ;

    if (FMath::IsNearlyZero(ZDifference, 0.1f))
    {
        return;
    }

    FVector PlayerLocation =
        Player->GetActorLocation();

    PlayerLocation.Z = MimicZ;

    Player->SetActorLocation(
        PlayerLocation,
        false
    );

    PlayerMovement->Velocity.Z =
        MimicMovement->Velocity.Z;
}

void UIllusionMovementComponent::StopBothCharacters()
{
    if (PlayerMovement)
    {
        PlayerMovement->StopMovementImmediately();
    }

    if (MimicMovement)
    {
        MimicMovement->StopMovementImmediately();
    }
}

bool UIllusionMovementComponent::IsPlayerMoving() const
{
    if (!PlayerMovement)
    {
        return false;
    }

    return PlayerMovement->Velocity.Size2D() > 10.0f;
}

bool UIllusionMovementComponent::IsMimicMoving() const
{
    if (!MimicMovement)
    {
        return false;
    }

    return MimicMovement->Velocity.Size2D() > 10.0f;
}