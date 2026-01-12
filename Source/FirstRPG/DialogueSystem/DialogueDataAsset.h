// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDialogueLine
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText DialogueText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString SpeakerID;
};

UCLASS(BlueprintType)
class FIRSTRPG_API UDialogueDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	//目前只打算有文本实例，如果之后还有其它如声音，字体等可以添加在这里
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FDialogueLine> DialogueLines;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FString RelativeQuestID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int Stage;
};
