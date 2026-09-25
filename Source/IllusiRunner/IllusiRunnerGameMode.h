// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "IllusiRunnerPlayerController.h"
#include "DeathScreen.h"

#include "IllusiRunnerGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AIllusiRunnerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AIllusiRunnerGameMode();
	virtual void BeginPlay() override;

	void PlayerDied();

	UPROPERTY(EditAnywhere, Category = "DeathScreenWidget")
	TSubclassOf<UDeathScreen> DeathScreenWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "DeathScreenWidget")
	UDeathScreen* DeathScreenWidget;

	AIllusiRunnerPlayerController* IllusiRunnerPlayerController;
};



