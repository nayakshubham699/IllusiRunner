// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "IllusiRunnerPlayerController.h"
#include "DeathScreen.h"
#include "PauseWidget.h"
#include "PlayerInteractWidget.h"

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPauseWidget> PauseWidgetClass;

	UPROPERTY(VisibleAnywhere)
	UPauseWidget* PauseWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerInteractWidget> PlayerInteractWidgetClass;

	UPROPERTY(VisibleAnywhere)
	UPlayerInteractWidget* PlayerInteractWidget;

	float PlayerTalkSeconds = 5.0f;

	int32 PlayerTalkedSeconds;

	FTimerHandle PlayerTalkTime;

	void PauseDisplayToggle(bool bPauseVisible);

	void OnPlayerTalkTimeOut();
};



