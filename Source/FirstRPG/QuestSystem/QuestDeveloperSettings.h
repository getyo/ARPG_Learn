// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "QuestTargetCondition.h"
#include "QuestDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Quest System Settings"))
class FIRSTRPG_API UQuestDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category = "QuestTargetCondition", meta = (RequiredMetaData,AllowPrivateAccess = "true"))
	TMap<E_QuestTargetConditionType, TSubclassOf<UQuestTargetCondition>> ConditionMap;
public:

	
	inline static const UQuestDeveloperSettings* Get() { return GetDefault<UQuestDeveloperSettings>(); }
	inline const TMap<E_QuestTargetConditionType, TSubclassOf<UQuestTargetCondition>>  * GetConditionMap () const{return &ConditionMap;} 
};