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

	UPROPERTY(Config, EditAnywhere, Category = "Quest", meta = (RequiredMetaData,AllowPrivateAccess = "true"))
	TMap<FGameplayTag, TSubclassOf<UQuestTargetCondition>> ConditionMap;
	UPROPERTY(Config, EditAnywhere, Category = "Quest", meta = (RequiredMetaData,AllowPrivateAccess = "true"))
	TMap<FString,FGameplayTag> QuestTag2IDMap;
	
public:

	
	inline static const UQuestDeveloperSettings* Get() { return GetDefault<UQuestDeveloperSettings>(); }
	inline const TMap<FGameplayTag, TSubclassOf<UQuestTargetCondition>>  * GetConditionMap () const{return &ConditionMap;} 
	inline const TMap<FString,FGameplayTag> * GetQuesID2TagMap() const{return &QuestTag2IDMap;}
};