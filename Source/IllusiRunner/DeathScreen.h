// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "DeathScreen.generated.h"

/**
 * 
 */
UCLASS()
class ILLUSIRUNNER_API UDeathScreen : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UTextBlock* GameOverText;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UTextBlock* DeathText;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* ReplayButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReplayButtonClick();
};
