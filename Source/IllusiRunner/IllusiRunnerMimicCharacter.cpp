// Fill out your copyright notice in the Description page of Project Settings.


#include "IllusiRunnerMimicCharacter.h"

AIllusiRunnerMimicCharacter::AIllusiRunnerMimicCharacter()
{

}

void AIllusiRunnerMimicCharacter::ReceiveMirrorMove(FVector WorldDirection)
{
	AddMovementInput(WorldDirection);
}

void AIllusiRunnerMimicCharacter::ReceiveMirrorLook(float X_Rotation, float Y_Rotation)
{
	DoLook(-X_Rotation, Y_Rotation);
}


