#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveGameInterface.generated.h"

struct FItemSaveData;
struct FCharacterSaveData;
struct FPlayerSaveData;

// ==================== 1. 物品存档接口 ====================
UINTERFACE(MinimalAPI, Blueprintable)
class UItemSaveInterface : public UInterface { GENERATED_BODY() };

class FIRSTRPG_API IItemSaveInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SaveGame")
	void LoadItemData(const FItemSaveData& Data);
    
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SaveGame")
	void SaveItemData(FItemSaveData& Data);
};

// ==================== 2. 角色存档接口 ====================
UINTERFACE(MinimalAPI, Blueprintable)
class UCharacterSaveInterface : public UInterface { GENERATED_BODY() };

class FIRSTRPG_API ICharacterSaveInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SaveGame")
	void LoadCharacterData(const FCharacterSaveData& Data);
    
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SaveGame")
	void SaveCharacterData(FCharacterSaveData& Data);
};

// ==================== 3. 玩家存档接口（可继承角色接口） ====================
UINTERFACE(MinimalAPI, Blueprintable)
class UPlayerSaveInterface : public UInterface { GENERATED_BODY() };

class FIRSTRPG_API IPlayerSaveInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SaveGame")
	void LoadPlayerData(const FPlayerSaveData& Data);
    
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SaveGame")
	void SavePlayerData(FPlayerSaveData& Data);
};