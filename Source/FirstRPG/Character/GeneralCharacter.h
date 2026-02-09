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
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Character")
	FGameplayTag CharacterTag;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Character")
	FString CharacterName;
	//仅作显示用
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Character")
	FString DisplayName;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline FString GetCharacterID() const{return CharacterID;}
	inline FString GetCharacterName() const{ return CharacterName; }
	inline FString GetDisplayName() const{return DisplayName;}
	inline FGameplayTag GetCharacterTag() const{return CharacterTag;}

private:
	FString CharacterID = "";
};
