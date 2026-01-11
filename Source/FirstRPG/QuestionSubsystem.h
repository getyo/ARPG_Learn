// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestionSubsystem.generated.h"

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
};


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
		return QuestInfos.Find(QuestID) != nullptr;
	}
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Quest")
	FS_QuestTarget GetNextQeustTarget(const FString& QuestID,int Stage);
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Quest")
	FS_QuestTarget GetQeustTarget(const FString& QuestID,int Stage);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	UDataTable* DataTableRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FString> ActiveQuests;
private:
	TMap<FString,FS_QuestInfo> QuestInfos;
	void ReadQuestion();
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
