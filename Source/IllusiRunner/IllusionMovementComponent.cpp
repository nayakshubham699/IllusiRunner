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
    }
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
        bMovementInputActive = false;
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

    Player->AddMovementInput(PlayerDirection);
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

    FVector MimicLocation =
        Mimic->GetActorLocation();

    const float TargetX =
        PlayerLocation.X;

    const float TargetY =
        -PlayerLocation.Y;

    if (!FMath::IsNearlyEqual(MimicLocation.X, TargetX, 0.01f) ||
        !FMath::IsNearlyEqual(MimicLocation.Y, TargetY, 0.01f))
    {
        MimicLocation.X = TargetX;
        MimicLocation.Y = TargetY;

        Mimic->SetActorLocation(
            MimicLocation,
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

    Mimic->SetActorRotation(MimicRotation);
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