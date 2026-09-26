// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class ILLUSIRUNNER_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* ReplayButton;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* ExitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* ResumeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* MainMenuButton;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UTextBlock* PauseText;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReplayButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UFUNCTION()
	void OnResumeButtonClicked();

	UFUNCTION()
	void OnMainMenuButtonClicked();


	void SetPauseText(FString PausingText);
};
