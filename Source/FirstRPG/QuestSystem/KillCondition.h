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
	// 需求数（配置值，不随进度改变）
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="KillCondition")
	int32 _RequiredCount = 0;
	// 剩余数（随击杀递减），读档时用它恢复进度
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="KillCondition")
	int32 _RemainingCount = 0;
	virtual bool ConditionPassed(const FS_QuestTargetData& QuestTargetData) override;
	// ===== 存档进度接口 =====
	virtual int32 GetSaveProgress() const override { return _RemainingCount; }
	virtual void ApplySaveProgress(int32 InProgress) override
	{
		_RemainingCount = FMath::Max(InProgress, 0);
		HasPassed = (_RemainingCount <= 0);
	}
protected:
	virtual void SetDefaultProperties(FGameplayTag TargetTag, int RequestedNum = 0, UObject* Outer = nullptr) override
	{
		this->_TargetTag = TargetTag;
		this->_RequiredCount = RequestedNum;
		this->_RemainingCount = RequestedNum;
	}
};
