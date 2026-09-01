// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "FirstRPG/Character/CharacterFactionType.h"
#include "FirstRPG/SaveSystem/SaveGameInterface.h"
#include "GeneralCharacter.generated.h"

UCLASS(Abstract, Blueprintable,BlueprintType)
class FIRSTRPG_API AGeneralCharacter : public ACharacter,public ICharacterSaveInterface
{
	GENERATED_BODY()

public:
	AGeneralCharacter();
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PreDuplicate(FObjectDuplicationParameters& DupParams) override;
	void RegisterCharacter();
	void UnRegisterCharacter();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Character")
	FGameplayTag CharacterTag;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Character")
	FString CharacterName;
	//仅作显示用
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Character")
	FString DisplayName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Character")
	ECharacterFaction CharacterFaction = ECharacterFaction::None;
	bool IsStatic = true;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline FString GetCharacterID() const{return CharacterID;}
	UFUNCTION(BlueprintCallable,Category = "Character")
	inline void SetCharacterName(FString Name){this->CharacterName = Name;}
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline FString GetCharacterName() const{ return CharacterName; }
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline FString GetDisplayName() const{return DisplayName;}
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline FGameplayTag GetCharacterTag() const{return CharacterTag;}
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Character")
	inline ECharacterFaction GetCharacterFaction() const{return CharacterFaction;}
	UFUNCTION(BlueprintCallable, Category = "Character")
	static AGeneralCharacter* SpawnCharacter(UObject* WorldContextObject, TSubclassOf<AGeneralCharacter> CharacterClass,
	                                         FTransform SpawnTransform);
	virtual void SaveCharacterData_Implementation(FCharacterSaveData& Data) override;
	virtual void LoadCharacterData_Implementation(const FCharacterSaveData& Data) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	FString CharacterID = "";
};
