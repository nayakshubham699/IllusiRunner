// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathScreen.h"

#include "IllusiRunnerGameInstance.h"


void UDeathScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReplayButton)
	{
		ReplayButton->OnClicked.AddDynamic(this, &UDeathScreen::OnReplayButtonClick);
	}
}

void UDeathScreen::OnReplayButtonClick()
{
	UGameInstance* GameInstance = GetGameInstance();
	UIllusiRunnerGameInstance* IllusiRunnerGameInstance = Cast<UIllusiRunnerGameInstance>(GameInstance);

	if (IllusiRunnerGameInstance)
	{
		IllusiRunnerGameInstance->RestartCurrentLevel();
	}

	//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}
