// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "QuestTargetCondition.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestionSubsystem.generated.h"

//任务系统内部的目标结构体
USTRUCT(BlueprintType)
struct FS_QuestTarget
{	
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int StageInt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString TargetName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString TargetDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	//在界面编辑的任务条件
	TArray<FS_QuestTargetConditionInfo> EditedQuestTargetConditions;
	//真正执行期间的条件，在任务子系统读入时根据前者生成
	UPROPERTY()
	TArray<UQuestTargetCondition*> Conditions;
};

//任务结构
USTRUCT(BlueprintType)
struct FS_QuestInfo: public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FS_QuestTarget> Targets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString Description;
};


//玩家持有的目标结构，与任务系统的目标不同在于，玩家持有的目标是为了知道自己的进度
//而任务系统的目标数组只做查询用处。
USTRUCT(BlueprintType)
struct FS_PlayerHoldTarget
{	
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int StageInt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString TargetName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString TargetDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool IsCompleted;
};

//玩家控制器持有的，真的的在玩家那里记录游戏任务进度的类型
USTRUCT(BlueprintType)
struct FS_PlayerQuestHandler
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool IsFinished;
	//只保存当前进度的目标
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FS_PlayerHoldTarget CurrentTarget;
};


UCLASS(BlueprintType, Blueprintable,Category = "Quest")
class FIRSTRPG_API UQuestionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Quest")
	FS_QuestInfo GetQuest(const FString& QuestID);
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Quest")
	TArray<FString> GetAllQuestID();
	
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Quest")
	inline bool HasQuest(const FString& QuestID)
	{
		return _QuestInfos.Find(QuestID) != nullptr;
	}
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Quest")
	FS_QuestTarget GetNextQeustTarget(const FString& QuestID,int Stage);
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Quest")
	FS_QuestTarget GetQeustTarget(const FString& QuestID,int Stage);
	UFUNCTION(BlueprintCallable,Category = "QuestTargetCondition")
	void BroadcastFinish(FS_QuestTargetData QuestTargetData);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	UDataTable* DataTableRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FString> ActiveQuests;
private:
	TMap<FString,FS_QuestInfo> _QuestInfos;
	void ReadQuestion();
	UFUNCTION()
	void DeliverTargetCheck(const FS_QuestTargetData & QuestTargetData);
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	bool ShouldCreateSubsystem(UObject* Outer) const override{
		// 如果当前的类【正好就是】这个 C++ 类（而不是它的蓝图子类），就返回 false
		if (GetClass() == UQuestionSubsystem::StaticClass())
		{
			return false;
		}
		return Super::ShouldCreateSubsystem(Outer);
	}
};
