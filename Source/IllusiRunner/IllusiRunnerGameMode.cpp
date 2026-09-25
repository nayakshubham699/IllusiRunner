// Copyright Epic Games, Inc. All Rights Reserved.

#include "IllusiRunnerGameMode.h"

#include "Kismet/GameplayStatics.h"


AIllusiRunnerGameMode::AIllusiRunnerGameMode()
{
	// stub
}

void AIllusiRunnerGameMode::BeginPlay()
{
	Super::BeginPlay();

	IllusiRunnerPlayerController = Cast<AIllusiRunnerPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	IllusiRunnerPlayerController->SetInputMode(FInputModeGameOnly());

}

void AIllusiRunnerGameMode::PlayerDied()
{
	UE_LOG(LogTemp, Display, TEXT("Called AIllusiRunnerGameMode::PlayerDied()"));

	if (IllusiRunnerPlayerController)
	{
		UE_LOG(LogTemp, Display, TEXT("Got IllusiRunnerPlayerController"));
		DeathScreenWidget = CreateWidget<UDeathScreen>(IllusiRunnerPlayerController, DeathScreenWidgetClass);


		if (DeathScreenWidget)
		{
			DeathScreenWidget->AddToViewport();
			UE_LOG(LogTemp, Display, TEXT("DeathScreenCreated"));

			IllusiRunnerPlayerController->SetInputMode(FInputModeUIOnly());
			IllusiRunnerPlayerController->bShowMouseCursor = true;
		}
	}
}
