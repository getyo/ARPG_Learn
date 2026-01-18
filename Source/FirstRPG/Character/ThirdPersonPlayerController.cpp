// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonPlayerController.h"

TMap<FString,FS_PlayerQuestHandler> AThirdPersonPlayerController::GetAllActiveQuest_Implementation()
{
	TMap<FString,FS_PlayerQuestHandler> ActiveQuests;
	// 在这里写你的逻辑
	return ActiveQuests;
}

bool AThirdPersonPlayerController::GotoNextStage_Implementation(const FString& QuestID)
{
	return true;
}

void AThirdPersonPlayerController::CompleteQuest_Implementation(const FString& QuestID)
{
}
