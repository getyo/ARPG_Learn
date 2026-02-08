// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "FirstRPG/QuestSystem/QuestRelativeInterface.h"
#include "GeneralCharacter.generated.h"

UCLASS(Abstract, Blueprintable,BlueprintType)
class FIRSTRPG_API AGeneralCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGeneralCharacter();
	// Sets default values for this character's properties
	AGeneralCharacter(const FString &CharacterName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Quest")
	bool QuestRelative = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline FString GetCharacterID() const
	{
		return CharacterID;
	}

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Character")
	FGameplayTag CharacterTag;
	//仅作显示用，真正内部使用的是ID和Tag
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Character")
	FString DisplayName;
private:
	FString CharacterID = "";
};
