// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/TextBlock.h"

#include "PlayerInteractWidget.generated.h"

/**
 * 
 */
UCLASS()
class ILLUSIRUNNER_API UPlayerInteractWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UTextBlock* PlayerTalk;

	void SetPlayerTalkText(FString TalkText);
};
