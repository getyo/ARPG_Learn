// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestionSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FS_QuestInfo: public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int Stage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int Category;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString Discirption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool IsActive;
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
	
	UFUNCTION(BlueprintCallable, Category = "Quest")
	inline void ActiveQuest(const FString& QuestID)
	{
		QuestInfos.Find(QuestID)->IsActive = true;
		ActiveQuests.Add(QuestID);
	}
	UFUNCTION(BlueprintCallable, Category = "Quest")
	inline void UntrackedQuest(const FString& QuestID)
	{
		QuestInfos.Find(QuestID)->IsActive = false;
		ActiveQuests.Remove(QuestID);
	}
	UFUNCTION(BlueprintCallable, Category = "Quest")
	inline void SetQuestCompleted(const FString& QuestID)
	{
		QuestInfos.Find(QuestID)->IsCompleted = true;
	}
	inline void SetQuestUnFinished(const FString& QuestID)
	{
		QuestInfos.Find(QuestID)->IsCompleted = false;
	}
	
	
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
