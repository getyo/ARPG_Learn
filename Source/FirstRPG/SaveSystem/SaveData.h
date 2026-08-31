#pragma once
#include "CoreMinimal.h"
#include "FirstRPG/Character/CharacterFactionType.h"
#include "FirstRPG/Item/Equipment/EquipmentDataType.h"
#include "SaveData.generated.h"

USTRUCT(BlueprintType)
struct FActorSaveData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FRotator Rotation = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FVector Scale = FVector(1.f);

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    bool Dead = false;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    bool IsStatic = false;
    
    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FString Name;
};

USTRUCT(BlueprintType)
struct FItemSaveData : public FActorSaveData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FDataTableRowHandle ItemInfoRowHandler;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TSubclassOf<class AGeneralItemActor> ItemClass;
};

USTRUCT(BlueprintType)
struct FCharacterSaveData : public FActorSaveData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FS_DefaultEquipmentStatus EquipStatus;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    float Health = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    float MaxHealth = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    float MaxStamina = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    int32 Level = 1;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    bool CanBeKilled = true;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    ECharacterFaction Faction;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TSubclassOf<class AGeneralCharacter> CharacterClass;
};

USTRUCT(BlueprintType)
struct FBagItemData
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FDataTableRowHandle ItemInfoRowHandler;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TSubclassOf<class AGeneralItemActor> ItemClass;
};
USTRUCT(BlueprintType)
struct FPlayerSaveData : public FCharacterSaveData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TArray<FBagItemData> Equipments;
};

