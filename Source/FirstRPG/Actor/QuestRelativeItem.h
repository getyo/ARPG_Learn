// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralActor.h"
#include "FirstRPG/QuestSystem/QuestRelativeInterface.h"
#include "FirstRPG/QuestSystem/QuestionSubsystem.h"
#include "FirstRPG/Character/Player/ThirdPersonPlayerController.h"
#include "QuestRelativeItem.generated.h"

USTRUCT(BlueprintType)
struct FS_ItemHoldQuestTargets
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FS_NPCHoldTarget> Targets;
};

UCLASS()
class FIRSTRPG_API AQuestRelativeItem : public AGeneralActor,public IQuestRelativeInterface
{
	GENERATED_BODY()
public:
	virtual bool IsQuestRelative_Implementation() override;
	virtual TArray<FGameplayTag> GetRelativeQuest_Implementation() override;
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable,Category = "Quest")
	void SendTargetFinish(const FGameplayTag& ActionTag, int Cnt, AController * TargetInstigator);
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Quest")
	TMap<FGameplayTag,FS_ItemHoldQuestTargets> RelativeQuestMap;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Quest")
	bool IsQuestActor = true;
	//virtual void BeginPlay() override;
private:
	AThirdPersonPlayerController * PlayerController = nullptr;
	UQuestionSubsystem * QuestSubsystem = nullptr;
};
