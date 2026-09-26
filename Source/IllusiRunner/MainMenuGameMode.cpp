// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

#include "Kismet/GameplayStatics.h"

void AMainMenuGameMode::BeginPlay()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);


	if (PlayerController)
	{
		
		
		PlayerController->SetInputMode(FInputModeGameOnly());
		MainMenuWidget = CreateWidget<UMainMenuWidget>(PlayerController, MainMenuWidgetClass);

		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->bShowMouseCursor = true;
		}
	}
}
