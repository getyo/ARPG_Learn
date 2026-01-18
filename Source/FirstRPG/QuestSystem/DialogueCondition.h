// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestTargetCondition.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "DialogueCondition.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TalkConditionTag)

UCLASS(Blueprintable,BlueprintType)
class FIRSTRPG_API UDialogueCondition : public UQuestTargetCondition
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="DialogueCondition")
	FGameplayTag _TargetTag;
	UDialogueCondition();
	virtual bool ConditionPassed(const FS_QuestTargetData& QuestTargetData) override;
protected:
	virtual void SetDefaultProperties(FGameplayTag TargetTag, int RequestedNum = 0, UObject* Outer = nullptr) override
	{
		_TargetTag = TargetTag;	
	}
};
