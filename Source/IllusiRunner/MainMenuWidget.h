// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ILLUSIRUNNER_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* PlayButton;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* ExitButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnPlayGameButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

};
