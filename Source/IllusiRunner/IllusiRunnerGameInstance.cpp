// Fill out your copyright notice in the Description page of Project Settings.


#include "IllusiRunnerGameInstance.h"

#include "Kismet/GameplayStatics.h"

#include "IllusiRunnerGameMode.h"

void UIllusiRunnerGameInstance::ChangeLevel(int32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("ChangeLevel called with Index = %d"), Index);

	if (Index > 0 && Index <= LastLevelIndex)
	{
		CurrentLevelIndex = Index;

		FString LevelNameString =
			FString::Printf(TEXT("Main_Level_%d"), CurrentLevelIndex);

		UE_LOG(LogTemp, Warning, TEXT("Opening level: %s"), *LevelNameString);

		UGameplayStatics::OpenLevel(GetWorld(), *LevelNameString);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid level index: %d"), Index);
	}
}
void UIllusiRunnerGameInstance::LoadNextlevel()
{
	UE_LOG(LogTemp, Warning, TEXT("LoadNextlevel: CurrentLevelIndex = %d, LastLevelIndex = %d"),
		CurrentLevelIndex, LastLevelIndex);

	if (CurrentLevelIndex < LastLevelIndex)
	{
		LavaDeath = 0;

		UE_LOG(LogTemp, Warning, TEXT("Opening next level: %d"), CurrentLevelIndex + 1);

		ChangeLevel(CurrentLevelIndex + 1);
	}
	else
	{
		LavaDeath = 0;

		UE_LOG(LogTemp, Warning, TEXT("Reached last level. Restarting game."));

		//RestartGame();

		AIllusiRunnerGameMode* GameMode =
			Cast<AIllusiRunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (GameMode)
		{
			GameMode->LastLevelCompleted();
		}
	}
}

void UIllusiRunnerGameInstance::RestartCurrentLevel()
{
	ChangeLevel(CurrentLevelIndex);
}

void UIllusiRunnerGameInstance::RestartGame()
{
	ChangeLevel(1);
}
