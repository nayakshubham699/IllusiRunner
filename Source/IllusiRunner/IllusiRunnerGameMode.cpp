// Copyright Epic Games, Inc. All Rights Reserved.

#include "IllusiRunnerGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "IllusiRunnerPlayerController.h"
#include "IllusiRunnerGameInstance.h"


AIllusiRunnerGameMode::AIllusiRunnerGameMode()
{
	// stub
}

void AIllusiRunnerGameMode::BeginPlay()
{
	Super::BeginPlay();

	IllusiRunnerPlayerController = Cast<AIllusiRunnerPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	IllusiRunnerPlayerController->SetInputMode(FInputModeGameOnly());

	/*DeathScreenWidget = CreateWidget<UDeathScreen>(IllusiRunnerPlayerController, DeathScreenWidgetClass);
	if (DeathScreenWidget)
	{
		DeathScreenWidget->AddToViewport();
		UE_LOG(LogTemp, Display, TEXT("DeathScreenCreated"));

		DeathScreenWidget->GameOverText->SetVisibility(ESlateVisibility::Hidden);
		DeathScreenWidget->DeathText->SetVisibility(ESlateVisibility::Hidden);
		DeathScreenWidget->ReplayButton->SetVisibility(ESlateVisibility::Hidden);
	}
	*/


	AIllusiRunnerPlayerController* PlayerController = Cast<AIllusiRunnerPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PlayerController)
	{
		PauseWidget = CreateWidget<UPauseWidget>(PlayerController, PauseWidgetClass);
		if (PauseWidget)
		{
			PauseWidget->SetPauseText(FString::Printf(TEXT("Paused")));
			PauseWidget->PauseText->SetVisibility(ESlateVisibility::Hidden);
			PauseWidget->ReplayButton->SetVisibility(ESlateVisibility::Hidden);
			PauseWidget->ExitButton->SetVisibility(ESlateVisibility::Hidden);
			PauseWidget->ResumeButton->SetVisibility(ESlateVisibility::Hidden);
			PauseWidget->MainMenuButton->SetVisibility(ESlateVisibility::Hidden);
			PauseWidget->AddToViewport();
		}

		PlayerInteractWidget = CreateWidget<UPlayerInteractWidget>(PlayerController, PlayerInteractWidgetClass);
		if (PlayerInteractWidget)
		{
			PlayerInteractWidget->PlayerTalk->SetVisibility(ESlateVisibility::Hidden);
			PlayerInteractWidget->AddToViewport();
		}
	}

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	UIllusiRunnerGameInstance* IllusiRunnerGameInstance = Cast<UIllusiRunnerGameInstance>(GameInstance);
	if (IllusiRunnerGameInstance)
	{
		if (IllusiRunnerGameInstance->LavaDeath == 6)
		{
			PlayerInteractWidget->SetPlayerTalkText(FString::Printf(TEXT("Explorer: Please be Patient")));
			
			GetWorldTimerManager().SetTimer(PlayerTalkTime, this, &AIllusiRunnerGameMode::OnPlayerTalkTimeOut, PlayerTalkSeconds, true);

			PlayerInteractWidget->PlayerTalk->SetVisibility(ESlateVisibility::Visible);

		}

		else if (IllusiRunnerGameInstance->LavaDeath == 10)
		{
			PlayerInteractWidget->SetPlayerTalkText(FString::Printf(TEXT("Explorer: You are Really bad at it aren't you")));

			GetWorldTimerManager().SetTimer(PlayerTalkTime, this, &AIllusiRunnerGameMode::OnPlayerTalkTimeOut, PlayerTalkSeconds, true);

			PlayerInteractWidget->PlayerTalk->SetVisibility(ESlateVisibility::Visible);

		}

		else if (IllusiRunnerGameInstance->LavaDeath == 14)
		{
			PlayerInteractWidget->SetPlayerTalkText(FString::Printf(TEXT("Explorer: Are you Doing this on purpose")));

			GetWorldTimerManager().SetTimer(PlayerTalkTime, this, &AIllusiRunnerGameMode::OnPlayerTalkTimeOut, PlayerTalkSeconds, true);

			PlayerInteractWidget->PlayerTalk->SetVisibility(ESlateVisibility::Visible);

		}

		else if (IllusiRunnerGameInstance->LavaDeath == 20)
		{
			PlayerInteractWidget->SetPlayerTalkText(FString::Printf(TEXT("Explorer: I am done talking with You")));

			GetWorldTimerManager().SetTimer(PlayerTalkTime, this, &AIllusiRunnerGameMode::OnPlayerTalkTimeOut, PlayerTalkSeconds, true);

			PlayerInteractWidget->PlayerTalk->SetVisibility(ESlateVisibility::Visible);

		}
	}
}

void AIllusiRunnerGameMode::PlayerDied()
{
	UE_LOG(LogTemp, Display, TEXT("Called AIllusiRunnerGameMode::PlayerDied()"));

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	UIllusiRunnerGameInstance* IllusiRunnerGameInstance = Cast<UIllusiRunnerGameInstance>(GameInstance);

	if (IllusiRunnerGameInstance)
	{
		IllusiRunnerGameInstance->LavaDeath += 1;
	}
	if (IllusiRunnerPlayerController)
	{
		UE_LOG(LogTemp, Display, TEXT("Got IllusiRunnerPlayerController"));
		DeathScreenWidget = CreateWidget<UDeathScreen>(IllusiRunnerPlayerController, DeathScreenWidgetClass);


		if (DeathScreenWidget)
		{
			DeathScreenWidget->AddToViewport();
			UE_LOG(LogTemp, Display, TEXT("DeathScreenCreated"));

			UGameplayStatics::SetGamePaused(GetWorld(), true);
			IllusiRunnerPlayerController->SetInputMode(FInputModeUIOnly());
			IllusiRunnerPlayerController->bShowMouseCursor = true;


		}
	}
}

void AIllusiRunnerGameMode::PauseDisplayToggle(bool bPauseVisible)
{
	if (bPauseVisible)
	{
		PauseWidget->PauseText->SetVisibility(ESlateVisibility::Visible);

		PauseWidget->ReplayButton->SetVisibility(ESlateVisibility::Visible);

		PauseWidget->ExitButton->SetVisibility(ESlateVisibility::Visible);

		PauseWidget->MainMenuButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PauseWidget->PauseText->SetVisibility(ESlateVisibility::Hidden);

		PauseWidget->ReplayButton->SetVisibility(ESlateVisibility::Hidden);

		PauseWidget->ExitButton->SetVisibility(ESlateVisibility::Hidden);

		PauseWidget->MainMenuButton->SetVisibility(ESlateVisibility::Hidden);
	}

}

void AIllusiRunnerGameMode::OnPlayerTalkTimeOut()
{
	PlayerInteractWidget->PlayerTalk->SetVisibility(ESlateVisibility::Hidden);
}

void AIllusiRunnerGameMode::LastLevelCompleted()
{
	if (PauseWidget)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);

		AIllusiRunnerPlayerController* PlayerController = Cast<AIllusiRunnerPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = true;
		}
		PauseWidget->SetPauseText(FString::Printf(TEXT("Game Completed")));
		PauseWidget->PauseText->SetVisibility(ESlateVisibility::Visible);

		//PauseWidget->ReplayButton->SetVisibility(ESlateVisibility::Visible);
		PauseWidget->ExitButton->SetVisibility(ESlateVisibility::Visible);
		//PauseWidget->ResumeButton->SetVisibility(ESlateVisibility::Visible);
		PauseWidget->MainMenuButton->SetVisibility(ESlateVisibility::Visible);

		
	}
}
