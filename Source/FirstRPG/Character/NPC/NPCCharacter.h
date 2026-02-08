// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstRPG/Character/GeneralCharacter.h"
#include "FirstRPG/QuestSystem/QuestionSubsystem.h"
#include "FirstRPG/QuestSystem/QuestRelativeInterface.h"
#include "NPCCharacter.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FS_NPCHoldQuestTargets
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FS_NPCHoldTarget> Targets;
};

UCLASS()
class FIRSTRPG_API ANPCCharacter : public AGeneralCharacter,public IQuestRelativeInterface
{
	GENERATED_BODY()
public:
	virtual bool IsQuestRelative_Implementation() override;
	virtual TArray<FGameplayTag> GetRelativeQuest_Implementation() override;
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable,Category = "Quest")
	void SendTargetFinish(int Cnt,AController * TargetInstigator);
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Quest")
	TMap<FGameplayTag,FS_NPCHoldQuestTargets> RelativeQuestMap;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Quest")
	bool IsQuestActor = false;
	//virtual void BeginPlay() override;
private:
	AThirdPersonPlayerController * PlayerController = nullptr;
	UQuestionSubsystem * QuestSubsystem = nullptr;
};
