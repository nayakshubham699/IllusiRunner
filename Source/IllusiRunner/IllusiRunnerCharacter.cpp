// Copyright Epic Games, Inc. All Rights Reserved.

#include "IllusiRunnerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "IllusiRunner.h"
#include "IllusiRunnerMimicCharacter.h"
#include "Kismet/GameplayStatics.h"

AIllusiRunnerCharacter::AIllusiRunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AIllusiRunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (MimicClass)
	{
		Mimic = Cast<AIllusiRunnerMimicCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), MimicClass));
		if (Mimic)
		{
			UE_LOG(LogTemp, Display, TEXT(" Success to find a Mimic of class !"));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("'%s' Failed to find a Mimic of class '%s'!"), *GetNameSafe(this), *GetNameSafe(MimicClass));
		}
	}
}

void AIllusiRunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AIllusiRunnerCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AIllusiRunnerCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AIllusiRunnerCharacter::Look);
	}
	else
	{
		UE_LOG(LogIllusiRunner, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AIllusiRunnerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Move Player
	DoMove(MovementVector.X, MovementVector.Y);

	//UE_LOG(LogTemp, Display, TEXT("X = %f"), MovementVector.X);

	//UE_LOG(LogTemp, Display, TEXT("Y = %f"), MovementVector.Y);
	
	// Move Mimic
	if (Mimic)
	{
		Mimic->ReceiveMirrorMove(
			-MovementVector.X,
			MovementVector.Y
		);

	}
}

void AIllusiRunnerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();


	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);

	//UE_LOG(LogTemp, Display, TEXT("X = %f"), LookAxisVector.X);

	//UE_LOG(LogTemp, Display, TEXT("Y = %f"), LookAxisVector.Y);
	if (Mimic)
	{
		// route the input
		Mimic->ReceiveMirrorLook(LookAxisVector.X, LookAxisVector.Y);
	}
}

void AIllusiRunnerCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		// FRotator MirrorYawRotation(0, -Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		UE_LOG(LogTemp, Display, TEXT("ForwardDirection = %s"), *ForwardDirection.ToString());

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		UE_LOG(LogTemp, Display, TEXT("RightDirection = %s"), *RightDirection.ToString());


		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);

	}
}

void AIllusiRunnerCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AIllusiRunnerCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AIllusiRunnerCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
