// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "QuestTargetCondition.h"
#include "GlobalQuestTargetMessenger.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestTargetFinishedEvent, const FS_QuestTargetData&, Message);

UCLASS(BlueprintType, Blueprintable)
class FIRSTRPG_API UGlobalQuestTargetMessenger : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnQuestTargetFinishedEvent OnQuestTargetFinishedEvent;
};
