// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IllusiRunnerCharacter.h"
#include "IllusiRunnerMimicCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ILLUSIRUNNER_API AIllusiRunnerMimicCharacter : public AIllusiRunnerCharacter
{
	GENERATED_BODY()
	
public:
	AIllusiRunnerMimicCharacter();

	void ReceiveMirrorMove(float Forward, float Right);

	void ReceiveMirrorLook(float X_Rotation, float Y_Rotation);
};
