// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTargetCondition.h"
#include "NativeGameplayTags.h"
#include "InteractCondition.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTRPG_API UInteractCondition : public UQuestTargetCondition
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="DialogueCondition")
	FGameplayTag _TargetTag;
	UInteractCondition();
	virtual bool ConditionPassed(const FS_QuestTargetData& QuestTargetData) override;
protected:
	virtual void SetDefaultProperties(FGameplayTag TargetTag, int RequestedNum = 0, UObject* Outer = nullptr) override
	{
		_TargetTag = TargetTag;	
	}
};
