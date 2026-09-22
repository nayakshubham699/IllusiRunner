// Fill out your copyright notice in the Description page of Project Settings.


#include "IllusiRunnerMimicCharacter.h"

AIllusiRunnerMimicCharacter::AIllusiRunnerMimicCharacter()
{
	bIsMimic = true;
}

void AIllusiRunnerMimicCharacter::ReceiveMirrorMove(float Right, float Forward)
{
	DoMove(Right, Forward);
}

void AIllusiRunnerMimicCharacter::ReceiveMirrorLook(float X_Rotation, float Y_Rotation)
{
	DoLook(-X_Rotation, Y_Rotation);
}


