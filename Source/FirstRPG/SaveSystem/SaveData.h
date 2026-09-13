#pragma once
#include "CoreMinimal.h"
#include "FirstRPG/Character/CharacterFactionType.h"
#include "FirstRPG/Item/Equipment/EquipmentDataType.h"
#include "FirstRPG/QuestSystem/QuestionSubsystem.h"
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
    ECharacterFaction Faction = ECharacterFaction::Neutral;

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
// 单个任务目标内的条件进度快照。ConditionsRemaining 与 FS_QuestTarget::Conditions 下标一一对应，
// -1 表示该条件无可持久化进度（如纯事件判定的对话/交互条件）。
USTRUCT(BlueprintType)
struct FS_QuestConditionProgress
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    int32 StageInt = -1;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TArray<int32> ConditionsRemaining;
};

// 单个任务的条件进度集合（UHT 不允许 TArray 直接作为 TMap 的值，这里包一层）
USTRUCT(BlueprintType)
struct FS_QuestStagesProgress
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TArray<FS_QuestConditionProgress> Stages;
};

// 任务系统的存档块。任务定义本身来自数据表，这里只持久化运行时进度。
USTRUCT(BlueprintType)
struct FQuestSaveData
{
    GENERATED_BODY()

    // 玩家当前进行中的任务及其当前目标进度
    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TMap<FGameplayTag, FS_PlayerQuestHandler> ActiveQuests;

    // 当前聚焦任务
    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FGameplayTag FocusedQuest;

    // 各任务各阶段的条件计数器（例如击杀剩余数）
    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TMap<FGameplayTag, FS_QuestStagesProgress> ConditionProgress;
};

USTRUCT(BlueprintType)
struct FPlayerSaveData : public FCharacterSaveData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    TArray<FBagItemData> Equipments;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FQuestSaveData QuestData;
};

