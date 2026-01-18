// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "QuestTargetCondition.h"
#include "KillCondition.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(KillConditionTag)
/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FIRSTRPG_API UKillCondition : public UQuestTargetCondition
{
	GENERATED_BODY()
private:
	FCriticalSection Lock;
public:
	UKillCondition();
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="KillCondition")
	FGameplayTag _TargetTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="KillCondition")
	int32 _RequiredCount;
	virtual bool ConditionPassed(const FS_QuestTargetData& QuestTargetData) override;
protected:
	virtual void SetDefaultProperties(FGameplayTag TargetTag, int RequestedNum = 0, UObject* Outer = nullptr) override
	{
		this->_TargetTag = TargetTag;
		this->_RequiredCount = RequestedNum;
	}
};
