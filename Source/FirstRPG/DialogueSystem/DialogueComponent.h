// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueSubsystem.h"
#include "DialogueComponent.generated.h"


UCLASS( BlueprintType,Blueprintable,ClassGroup=(Dialogue), meta=(BlueprintSpawnableComponent) )
class FIRSTRPG_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UDialogueSubsystem* DialogueSubsystem = nullptr;
	bool InDialogue = false;
	
public:	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Dialogue")
	inline bool IsInDialogue() const
	{
		return InDialogue;
	}
	
	inline void SetInDialogue(bool Value)
	{
		this->InDialogue = Value;
	}
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool StartDialogue(const FString &QuestID,const int Stage)
	{
		if (DialogueSubsystem)
			return DialogueSubsystem->StartDialogue(QuestID,Stage);
		return false;
	}
};
