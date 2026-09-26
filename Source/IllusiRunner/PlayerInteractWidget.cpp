// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInteractWidget.h"

void UPlayerInteractWidget::SetPlayerTalkText(FString TalkText)
{
	if (PlayerTalk)
	{
		PlayerTalk->SetText(FText::FromString(TalkText));
	}
}
