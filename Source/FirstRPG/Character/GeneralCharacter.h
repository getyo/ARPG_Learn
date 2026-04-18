// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "GeneralCharacter.generated.h"

UENUM(NotBlueprintType)
enum class ECharacterFaction :uint8
{
	PlayerFriend,
	Neutral,
	Enemy,
	None
};

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
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Character")
	ECharacterFaction CharacterFaction = ECharacterFaction::None;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline FString GetCharacterID() const{return CharacterID;}
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline FString GetCharacterName() const{ return CharacterName; }
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline FString GetDisplayName() const{return DisplayName;}
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline FGameplayTag GetCharacterTag() const{return CharacterTag;}
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline ECharacterFaction GetCharacterFaction() const{return CharacterFaction;}
private:
	FString CharacterID = "";
};
