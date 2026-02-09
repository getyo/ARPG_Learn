// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FirstRPG/QuestSystem//QuestionSubsystem.h"
#include "ThirdPersonPlayerController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class FIRSTRPG_API AThirdPersonPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	//获取任务信息相关
	UFUNCTION(BlueprintCallable,Category = "Quest")
	TMap<FGameplayTag,FS_PlayerQuestHandler> GetAllActiveQuest();
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Quest")
	inline bool IsQuestActive(const FGameplayTag& QuestTag);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Quest")
	inline bool ActiveQuestEmpty();
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Quest")
	inline bool IsQuestFinished(const FGameplayTag& QuestTag);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Quest")
	inline FS_PlayerHoldTarget GetQuestHoldTarget(const FGameplayTag& QuestTag);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Quest")
	inline int GetQuestStageInt(const FGameplayTag& QuestTag);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Quest")
	inline bool IsQuestFocused(const FGameplayTag& QuestTag);
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Quest")
	inline bool HasFocusedQuest();
	
	//修改任务信息
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category = "Quest")
	void AddActiveQuest(const FGameplayTag& QuestTag);
	UFUNCTION(BlueprintCallable,Category = "Quest")
	void RemoveActiveQuest(const FGameplayTag& QuestTag);
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category = "Quest")
	void GotoNextStage(const FGameplayTag& QuestTag);
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category = "Quest")
	bool SetFocusedQuest(const FGameplayTag& QuestTag);
protected:
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quest")
	FGameplayTag FocusedQuest;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Quest")
	TMap<FGameplayTag,FS_PlayerQuestHandler> ActiveQuests;
private:
	UQuestionSubsystem * QuestSubsystem = nullptr;
};
