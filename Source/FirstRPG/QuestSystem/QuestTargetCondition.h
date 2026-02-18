// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestTargetCondition.generated.h"

//数据表格里面用户编辑的任务条件，不是真正操作的条件
//真正操作的条件根据这些信息工厂生成
USTRUCT(BlueprintType, Blueprintable)
struct FS_QuestTargetConditionInfo
{
	GENERATED_BODY()
	//用于定义当前的事件类型：击杀敌人，取得指定物品等
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayTag ActionTag;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayTag TargetTag; // 怪物ID、NPC标签等
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 RequiredCount = 0;
	
};


//触发任务目标完成的相关事件时需要传递的结构体
USTRUCT(BlueprintType)
struct FS_QuestTargetData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag QuestTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Stage = -1;
	//用于定义当前的事件类型：击杀敌人，取得指定物品等
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ActionTag;
	//用于传递完成的事件信息，解析交给各个条件类
	//eg:击杀事件传递敌人类型，对话事件传递对话的Quest，Stage
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag TargetTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetCnt = 0;
	//触发事件的角色
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AController * Instigator = nullptr;
	//用于传递上下文信息，只不过现在没用
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor * Context = nullptr;
};

UCLASS(BlueprintType, Blueprintable,Abstract)
class FIRSTRPG_API UQuestTargetCondition : public UObject
{
	GENERATED_BODY()
public:
	virtual bool ConditionPassed(const FS_QuestTargetData& QuestTargetData)
	{
		return true;
	} 
	static UQuestTargetCondition* QuestTargetConditionFactory(FGameplayTag ActionTag,FGameplayTag TargetTag,int RequestedNum=0,
		UObject* Outer = nullptr);
	static void InitQuestTargetCondition();
	UQuestTargetCondition();
	inline FGameplayTag GetQuestTargetConditionType() const
	{
		return _Type;
	}
	inline bool GetPassed() const
	{
		return HasPassed;
	}
private:
	static const TMap<FGameplayTag,TSubclassOf<UQuestTargetCondition>>* _E2Class;
protected:
	FGameplayTag _Type;
	bool HasPassed = false;
	virtual void SetDefaultProperties(FGameplayTag TargetTag,int RequestedNum=0,UObject* Outer = nullptr){}
	
};
