// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "IllusiRunnerGameInstance.h"
#include "IllusiRunnerPlayerController.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ReplayButton)
	{
		ReplayButton->OnClicked.AddDynamic(this, &UPauseWidget::OnReplayButtonClicked);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UPauseWidget::OnExitButtonClicked);
	}

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::OnResumeButtonClicked);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseWidget::OnMainMenuButtonClicked);
	}
}

void UPauseWidget::OnReplayButtonClicked()
{
	UGameInstance* GameInstance = GetGameInstance();
	UIllusiRunnerGameInstance* IllusiRunnerGameInstance = Cast<UIllusiRunnerGameInstance>(GameInstance);
	if (IllusiRunnerGameInstance)
	{
		IllusiRunnerGameInstance->RestartCurrentLevel();
	}
}

void UPauseWidget::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UPauseWidget::OnResumeButtonClicked()
{
	AIllusiRunnerPlayerController* PlayerController = Cast<AIllusiRunnerPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->PauseGame(false);

		UE_LOG(LogTemp, Display, TEXT("Game Resumed Button clicked!"));
	}
}

void UPauseWidget::OnMainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("MainMenu_Level")), false);
}

void UPauseWidget::SetPauseText(FString PausingText)
{
	if (PauseText)
	{
		PauseText->SetText(FText::FromString(PausingText));
	}
}
