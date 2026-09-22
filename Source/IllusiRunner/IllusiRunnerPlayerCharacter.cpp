// Fill out your copyright notice in the Description page of Project Settings.


#include "IllusiRunnerPlayerCharacter.h"

#include "IllusiRunnerMimicCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AIllusiRunnerPlayerCharacter::AIllusiRunnerPlayerCharacter()
{
}

void AIllusiRunnerPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PreviousPlayerZ = GetActorLocation().Z;
	if (Mimic)
	{
		PreviousMimicZ = Mimic->GetActorLocation().Z;
		UE_LOG(LogTemp, Display, TEXT("Mimic found"));
	}

	SharedZ = GetActorLocation().Z;

	bMimicIsHeightSource = false;
}

void AIllusiRunnerPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SyncCharacterHeight();
}

void AIllusiRunnerPlayerCharacter::SyncCharacterHeight()
{
    if (!Mimic)
    {
        return;
    }

    UCharacterMovementComponent* PlayerMovement = GetCharacterMovement();
    UCharacterMovementComponent* MimicMovement = Mimic->GetCharacterMovement();

    const bool bPlayerGrounded = PlayerMovement->IsMovingOnGround();
    const bool bMimicGrounded = MimicMovement->IsMovingOnGround();

    if (bPlayerGrounded && !bMimicGrounded)
    {
        MimicMovement->GravityScale = 0.0f;

        FVector MimicLocation = Mimic->GetActorLocation();
        MimicLocation.Z = GetActorLocation().Z;

        Mimic->SetActorLocation(
            MimicLocation,
            false,
            nullptr,
            ETeleportType::TeleportPhysics
        );

        MimicMovement->Velocity.Z = 0.0f;
    }
    else if (!bPlayerGrounded && bMimicGrounded)
    {
        PlayerMovement->GravityScale = 0.0f;

        FVector PlayerLocation = GetActorLocation();
        PlayerLocation.Z = Mimic->GetActorLocation().Z;

        SetActorLocation(
            PlayerLocation,
            false,
            nullptr,
            ETeleportType::TeleportPhysics
        );

        PlayerMovement->Velocity.Z = 0.0f;
    }
    else if (bPlayerGrounded && bMimicGrounded)
    {
        PlayerMovement->GravityScale = 1.0f;
        MimicMovement->GravityScale = 1.0f;
    }
}
