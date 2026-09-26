// Fill out your copyright notice in the Description page of Project Settings.


#include "IllusiRunnerGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UIllusiRunnerGameInstance::ChangeLevel(int32 Index)
{
	if (Index > 0 && Index <= LastLevelIndex)
	{
		CurrentLevelIndex = Index;

		FString LevelNameString = FString::Printf(TEXT("Main_Level_%d"), CurrentLevelIndex);

		UGameplayStatics::OpenLevel(GetWorld(), *LevelNameString);
	}
}

void UIllusiRunnerGameInstance::LoadNextlevel()
{
	if (CurrentLevelIndex < LastLevelIndex)
	{
		LavaDeath = 0;
		
		CurrentLevelIndex += CurrentLevelIndex;
		ChangeLevel(CurrentLevelIndex);

		
		
	}
	else
	{
		LavaDeath = 0;
		
		
		RestartGame();

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
