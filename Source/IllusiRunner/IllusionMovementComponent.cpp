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

    LastPlayerLocation = FVector::ZeroVector;
    LastMimicLocation = FVector::ZeroVector;

    bMovementInputActive = false;
    bPlayerJumping = false;

    bMimicHorizontalBlocked = false;
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
    }

    LastPlayerLocation =
        Player->GetActorLocation();

    LastMimicLocation =
        Mimic->GetActorLocation();
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

    UpdateMimicHorizontalMovement();

    UpdateVerticalMovement();

    LastPlayerLocation =
        Player->GetActorLocation();

    LastMimicLocation =
        Mimic->GetActorLocation();
}

void UIllusionMovementComponent::Move(FVector2D MovementInput)
{
    if (!Player || !Mimic)
    {
        return;
    }

    CurrentMovementInput = MovementInput;

    if (MovementInput.IsNearlyZero())
    {
        bMovementInputActive = false;

        FVector PlayerVelocity =
            PlayerMovement->Velocity;

        PlayerVelocity.X = 0.0f;
        PlayerVelocity.Y = 0.0f;

        PlayerMovement->Velocity =
            PlayerVelocity;

        FVector MimicVelocity =
            MimicMovement->Velocity;

        MimicVelocity.X = 0.0f;
        MimicVelocity.Y = 0.0f;

        MimicMovement->Velocity =
            MimicVelocity;

        return;
    }

    bMovementInputActive = true;

    if (!Player->GetController())
    {
        return;
    }

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

    Player->AddMovementInput(
        PlayerDirection
    );

    FVector MimicDirection =
        PlayerDirection;

    MimicDirection.Y *= -1.0f;

    Mimic->AddMovementInput(
        MimicDirection
    );

    if (MovementInput.IsNearlyZero())
    {
        bMovementInputActive = false;

        FVector PlayerVelocity =
            PlayerMovement->Velocity;

        PlayerVelocity.X = 0.0f;
        PlayerVelocity.Y = 0.0f;

        PlayerMovement->Velocity =
            PlayerVelocity;

        FVector MimicVelocity =
            MimicMovement->Velocity;

        MimicVelocity.X = 0.0f;
        MimicVelocity.Y = 0.0f;

        MimicMovement->Velocity =
            MimicVelocity;

        return;
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

void UIllusionMovementComponent::UpdateMimicHorizontalMovement()
{
    if (!Player || !Mimic)
    {
        return;
    }

    const FVector PlayerLocation =
        Player->GetActorLocation();

    const FVector MimicLocation =
        Mimic->GetActorLocation();

    const FVector PlayerDelta =
        PlayerLocation -
        LastPlayerLocation;

    const FVector MimicDelta =
        MimicLocation -
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

    const bool bPlayerActuallyMoved =
        PlayerMovementDistance > 0.1f;

    const bool bMimicActuallyMoved =
        MimicMovementDistance > 0.1f;

    if (bMovementInputActive)
    {
        const bool bPlayerBlocked =
            !bPlayerActuallyMoved &&
            PlayerMovement->Velocity.Size2D() < 1.0f;

        const bool bMimicBlocked =
            !bMimicActuallyMoved &&
            MimicMovement->Velocity.Size2D() < 1.0f;

        if (bPlayerBlocked && !bMimicBlocked)
        {
            FVector MimicVelocity =
                MimicMovement->Velocity;

            MimicVelocity.X = 0.0f;
            MimicVelocity.Y = 0.0f;

            MimicMovement->Velocity =
                MimicVelocity;

            FVector NewMimicLocation =
                MimicLocation;

            NewMimicLocation.X =
                PlayerLocation.X;

            NewMimicLocation.Y =
                -PlayerLocation.Y;

            Mimic->SetActorLocation(
                NewMimicLocation,
                false
            );
        }
        else if (bMimicBlocked && !bPlayerBlocked)
        {
            bMimicHorizontalBlocked = true;

            FVector PlayerVelocity =
                PlayerMovement->Velocity;

            PlayerVelocity.X = 0.0f;
            PlayerVelocity.Y = 0.0f;

            PlayerMovement->Velocity =
                PlayerVelocity;

            FVector NewPlayerLocation =
                PlayerLocation;

            NewPlayerLocation.X =
                MimicLocation.X;

            NewPlayerLocation.Y =
                -MimicLocation.Y;

            Player->SetActorLocation(
                NewPlayerLocation,
                false
            );
        }
    }

    if (bMimicHorizontalBlocked)
    {
        FVector CorrectedPlayerLocation =
            Player->GetActorLocation();

        CorrectedPlayerLocation.X =
            MimicLocation.X;

        CorrectedPlayerLocation.Y =
            -MimicLocation.Y;

        Player->SetActorLocation(
            CorrectedPlayerLocation,
            false
        );
    }

    const FRotator PlayerRotation =
        Player->GetActorRotation();

    FRotator MimicRotation =
        Mimic->GetActorRotation();

    MimicRotation.Pitch =
        PlayerRotation.Pitch;

    MimicRotation.Yaw =
        -PlayerRotation.Yaw;

    MimicRotation.Roll =
        PlayerRotation.Roll;

    Mimic->SetActorRotation(
        MimicRotation
    );
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

    PlayerLocation.Z =
        MimicZ;

    Player->SetActorLocation(
        PlayerLocation,
        false
    );

    PlayerMovement->Velocity.Z =
        MimicMovement->Velocity.Z;
}