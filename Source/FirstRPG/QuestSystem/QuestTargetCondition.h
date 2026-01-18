// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FirstRPG/Character//GeneralCharacter.h"
#include "QuestTargetCondition.generated.h"

UENUM(BlueprintType) // 允许在蓝图中使用
enum class E_QuestTargetConditionType : uint8
{
	None        UMETA(DisplayName = "None"),          // 显示名称
	Kill        UMETA(DisplayName = "Kill Monster"),  // 击杀
	Talk        UMETA(DisplayName = "Talk to NPC"),   // 对话
	Collect     UMETA(DisplayName = "Collect Item"),  // 收集
	Interact    UMETA(DisplayName = "Interact"),      // 交互
};

//数据表格里面用户编辑的任务条件，不是真正操作的条件
//真正操作的条件根据这些信息工厂生成
USTRUCT(BlueprintType, Blueprintable)
struct FS_QuestTargetConditionInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_QuestTargetConditionType Type;
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
	//前两个目前来说是一样的，因为动作类型目前来讲没有和完成方式区分
	//但是之后可能区分，为了未来兼容所以保留了两个成员
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_QuestTargetConditionType Type;
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
	AController * Instigator = nullptr;
	//用于传递上下文信息，只不过现在没用
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
	static UQuestTargetCondition* QuestTargetConditionFactory(E_QuestTargetConditionType Type,
		FGameplayTag TargetTag,
		int RequestedNum=0,
		UObject* Outer = nullptr);
	static void InitQuestTargetCondition();
	UQuestTargetCondition();
	inline E_QuestTargetConditionType GetQuestTargetConditionType() const
	{
		return _Type;
	}
private:
	static const TMap<E_QuestTargetConditionType,TSubclassOf<UQuestTargetCondition>>* _E2Class;
protected:
	E_QuestTargetConditionType _Type;
	virtual void SetDefaultProperties(FGameplayTag TargetTag,int RequestedNum=0,UObject* Outer = nullptr){}
	
};
