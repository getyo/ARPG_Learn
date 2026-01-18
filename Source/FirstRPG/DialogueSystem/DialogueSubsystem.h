// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogueDataAsset.h"
#include "FirstRPG/Character//ThirdPersonPlayerController.h"
#include "DialogueSubsystem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInQuestTargetDialogMap
{
	GENERATED_BODY()
	TMap<int,UDialogueDataAsset*> Map;
};

UCLASS()
class FIRSTRPG_API UDialogueSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	//维护从任务ID到任务对话的映射
	TMap<FString,FInQuestTargetDialogMap> DialogueAssetMap;
	FString CurrentQuest;
	int CurrentStage;
	size_t TextIt;
	AThirdPersonPlayerController* CurrentPlayerController = nullptr;
	UDialogueDataAsset * CurrentDialogueAsset = nullptr;
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	bool StartDialogue(const FString &QuestID, int Stage, AThirdPersonPlayerController* PlayerController);
	UFUNCTION(BlueprintCallable,Category = "Dialogue")
	bool EndDialogue();
	
	UFUNCTION(BlueprintCallable,Category = "Dialogue")
	inline FDialogueLine NextDialogue()
	{
		return CurrentDialogueAsset->DialogueLines[TextIt++];
	}
	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Dialogue")
	bool IsStageDialogueEnd() const
	{
		return TextIt >=  CurrentDialogueAsset->DialogueLines.Num()? true : false;
	}
};
