// Copyright Epic Games, Inc. All Rights Reserved.


#include "IllusiRunnerPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "IllusiRunner.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "IllusiRunnerGameMode.h"
#include "Kismet/GameplayStatics.h" 

void AIllusiRunnerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (IsLocalPlayerController() && ShouldUseTouchControls())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogIllusiRunner, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AIllusiRunnerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AIllusiRunnerPlayerController::TogglePause);
	}
}

bool AIllusiRunnerPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void AIllusiRunnerPlayerController::TogglePause()
{
	bool bIsPaused = UGameplayStatics::IsGamePaused(GetWorld());
	UE_LOG(LogTemp, Display, TEXT("TogglePause: bIsPaused = %s"), bIsPaused ? TEXT("true") : TEXT("false"));
	if (bIsPaused)
	{
		PauseGame(false);
	}
	else
	{
		PauseGame(true);
	}
}

void AIllusiRunnerPlayerController::PauseGame(bool bPause)
{
	AIllusiRunnerGameMode* IllusiRunnerGameMode = Cast<AIllusiRunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IllusiRunnerGameMode)
	{
		if (bPause)
		{
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			IllusiRunnerGameMode->PauseWidget->SetPauseText(FString::Printf(TEXT("Paused")));
			IllusiRunnerGameMode->PauseWidget->ResumeButton->SetVisibility(ESlateVisibility::Visible);

			IllusiRunnerGameMode->PauseDisplayToggle(true);
			SetInputMode(FInputModeUIOnly());
		}
		else
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);

			IllusiRunnerGameMode->PauseWidget->ResumeButton->SetVisibility(ESlateVisibility::Hidden);

			IllusiRunnerGameMode->PauseDisplayToggle(false);
			SetInputMode(FInputModeGameOnly());
		}
	}

	bShowMouseCursor = bPause;
}
