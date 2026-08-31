// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstRPG/Character/GeneralCharacter.h"
#include "SaveData.h"
#include "FirstRPG/Component/EquipmentComponent.h"
#include "SaveLoadSystem.generated.h"


UCLASS(Blueprintable, BlueprintType)
class FIRSTRPG_API USaveLoadSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	// 2. 将指针 Map 改为结构体值类型 Map
	TMap<FString, FItemSaveData> ItemsData;
	TMap<FString, FCharacterSaveData> CharacterData;
	FPlayerSaveData PlayerData;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, AGeneralItemActor*> ItemObjRef;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, AGeneralCharacter*> CharacterObjRef;
	
	void SpawnActors();
public:
	FString SlotName = "SaveSlot1";
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void Register(AActor *Obj);
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void UnRegister(AActor *Obj);
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadGame();
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void SaveGame();
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void DeleteSaveData();
};