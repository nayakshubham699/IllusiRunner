// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IllusiRunnerCharacter.h"
#include "IllusiRunnerPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ILLUSIRUNNER_API AIllusiRunnerPlayerCharacter : public AIllusiRunnerCharacter
{
	GENERATED_BODY()
	
public:
	AIllusiRunnerPlayerCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	float PreviousPlayerZ;

	UPROPERTY(VisibleAnywhere)
	float PreviousMimicZ;

	bool bMimicIsHeightSource;

	float SharedZ;

	bool bPlayerWasGrounded = false;
	bool bMimicWasGrounded = false;

	void SyncCharacterHeight();
};
