// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveLoadSystem.h"
#include "SaveGameContainer.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class FIRSTRPG_API USaveGameContainer : public USaveGame
{
	GENERATED_BODY()
public:
	// 存档中保存的物品数据表
	UPROPERTY(VisibleAnywhere, Category = "Save")
	TMap<FString, FItemSaveData> ItemsData;

	// 存档中保存的角色数据表
	UPROPERTY(VisibleAnywhere, Category = "Save")
	TMap<FString, FCharacterSaveData> CharacterData;
	
	UPROPERTY(VisibleAnywhere, Category = "Save")
	FPlayerSaveData PlayerData;

	// 存档结构版本：0 = 旧版存档（不含任务等后续数据）
	UPROPERTY(VisibleAnywhere, Category = "Save")
	int32 SaveVersion = 0;
};
