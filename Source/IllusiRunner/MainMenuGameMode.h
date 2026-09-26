// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "MainMenuWidget.h"

#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ILLUSIRUNNER_API AMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()
	

public:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "DeathScreenWidget")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "DeathScreenWidget")
	UMainMenuWidget* MainMenuWidget;

	
};
