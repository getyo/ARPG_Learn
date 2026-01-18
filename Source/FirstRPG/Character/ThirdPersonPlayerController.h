// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FirstRPG/QuestSystem//QuestionSubsystem.h"
#include "ThirdPersonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTRPG_API AThirdPersonPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Quest")
	TMap<FString,FS_PlayerQuestHandler> GetAllActiveQuest();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Quest")
	//当进行完最后一个阶段，也就是任务结束以后返回false
	bool GotoNextStage(const FString &QuestID);
	UFUNCTION(BlueprintNativeEvent, Category = "Quest")
	//当进行完最后一个阶段，也就是任务结束以后返回false
	void CompleteQuest(const FString &QuestID);
};
