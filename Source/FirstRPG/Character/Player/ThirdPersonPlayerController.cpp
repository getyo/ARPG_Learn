// Fill out your copyright notice in the Description page of Project Settings.
#include "ThirdPersonPlayerController.h"
#include "FirstRPG/SaveSystem/SaveData.h"
#include "FirstRPG/Character/GeneralCharacter.h"
#include "FirstRPG/Component/EquipmentComponent.h"
#include "FirstRPG/Debug/Debug.h"
#include "FirstRPG/Item/Equipment/MeleeWeapon.h"
#include "FirstRPG/Item/Equipment/RangeWeapon.h"
#include "FirstRPG/Item/Equipment/Shield.h"

void AThirdPersonPlayerController::QuestFinished_Implementation(const FGameplayTag& QuestTag)
{
	ActiveQuests[QuestTag].IsFinished = true;
	FocusedQuest = FGameplayTag::EmptyTag;
	ActiveQuests.Remove(QuestTag);
}

void AThirdPersonPlayerController::SavePlayerData_Implementation(FPlayerSaveData& Data)
{
	auto PlayerCharacter = Cast<AGeneralCharacter>(this->GetPawn());
	if (!PlayerCharacter)
	{
		CPP_LOG(Error,"Cannot save player data,Get Player Character failed.");
		return;
	}
	auto EquipmentCmp = PlayerCharacter->FindComponentByClass<UEquipmentComponent>();
	if (!EquipmentCmp)
	{
		CPP_LOG(Error,"Cannot save player data,Get Equipment Component failed.");
		return;
	}
	//保存背包内所有物品
	auto EquipmentInsts = EquipmentCmp->GetEquippableItems();
	TArray<FBagItemData> EquipmentSaveDataArray;
	FBagItemData EquipmentData;
	for (auto elem:EquipmentInsts)
	{
		EquipmentData.ItemClass = elem->GetBPClassToSpawn();
		EquipmentData.ItemInfoRowHandler = elem->GetDataTableRow();
		EquipmentSaveDataArray.Add(EquipmentData);
	}
	Data.Equipments = EquipmentSaveDataArray;
	//保存现在的装备
	auto EStatus = EquipmentCmp->GetCurEquipmentStatus();
	if (EStatus.MeleeWeapon)
		Data.EquipStatus.MeleeWeapon = EStatus.MeleeWeapon->GetItemInstance()->GetDataTableRow();
	if (EStatus.RangeWeapon)
		Data.EquipStatus.RangeWeapon = EStatus.RangeWeapon->GetItemInstance()->GetDataTableRow();
	if (EStatus.Armor)
		Data.EquipStatus.Armor = EStatus.Armor->GetItemInstance()->GetDataTableRow();
	if (EStatus.Shield)
		Data.EquipStatus.Shield = EStatus.Shield->GetItemInstance()->GetDataTableRow();
	//剩下的保存交给蓝图处理
}

void AThirdPersonPlayerController::LoadPlayerData_Implementation(const FPlayerSaveData& Data)
{
	auto PlayerCharacter = Cast<AGeneralCharacter>(this->GetPawn());
	if (!PlayerCharacter)
	{
		CPP_LOG(Error,"Cannot load player data,Get Player Character failed.");
		return;
	}
	auto EquipmentCmp = PlayerCharacter->FindComponentByClass<UEquipmentComponent>();
	if (!EquipmentCmp)
	{
		CPP_LOG(Error,"Cannot load player data,Get Equipment Component failed.");
		return;
	}
	EquipmentCmp->ClearBag();
	auto EStatus = Data.EquipStatus;
	//载入背包物品
	for (auto elem:Data.Equipments)
	{
		if (elem.ItemClass->IsChildOf(AEquippableItemActor::StaticClass()))
		{
			auto EquipmentInst = UEquipmentInstance::EquipmentInstanceFactory(elem.ItemInfoRowHandler);
			EquipmentCmp->AddEquippableItem(EquipmentInst);
			//恢复玩家装备状态
			if (!EStatus.MeleeWeapon.IsNull() && elem.ItemInfoRowHandler == EStatus.MeleeWeapon)
			{
				EquipmentCmp->SetEquippedMeleeWeapon(EquipmentInst);
			}
			if (!EStatus.RangeWeapon.IsNull() && elem.ItemInfoRowHandler == EStatus.RangeWeapon)
			{
				EquipmentCmp->SetEquippedRangeWeapon(EquipmentInst);
			}
			if (!EStatus.Armor.IsNull() && elem.ItemInfoRowHandler == EStatus.Armor)
			{
				EquipmentCmp->SetEquippedArmor(EquipmentInst);
			}
			if (!EStatus.Shield.IsNull() && elem.ItemInfoRowHandler == EStatus.Shield)
			{
				EquipmentCmp->SetEquippedShield(EquipmentInst);
			}
		}
	}
	
}

void AThirdPersonPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UGameInstance* GI = GetWorld()->GetGameInstance();
	if (GI)
	{
		QuestSubsystem= GI->GetSubsystem<UQuestionSubsystem>();
		if (!QuestSubsystem)
		{
			CPP_LOG(Error,"Cannot get Quest SubSystem");
		}
	}
}

TMap<FGameplayTag,FS_PlayerQuestHandler> AThirdPersonPlayerController::GetAllActiveQuest()
{
	return ActiveQuests;
}

inline bool AThirdPersonPlayerController::IsQuestActive(const FGameplayTag& QuestTag)
{
	return ActiveQuests.Contains(QuestTag);
}

inline bool AThirdPersonPlayerController::ActiveQuestEmpty()
{
	return ActiveQuests.IsEmpty();
}

inline bool AThirdPersonPlayerController::IsQuestFinished(const FGameplayTag& QuestTag)
{
	if (!IsQuestActive(QuestTag))
	{
		GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
			   FString::Printf(TEXT("Class : %s,Function: %s,Quest is not active: QuestTag: %s"),
				  *GetName(),*FString(__FUNCTION__),
				  *QuestTag.ToString()));
		return false;
	}
	return ActiveQuests[QuestTag].IsFinished;
}

FS_PlayerHoldTarget AThirdPersonPlayerController::GetQuestHoldTarget(const FGameplayTag& QuestTag)
{
	if (!IsQuestActive(QuestTag))
	{
		GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
			   FString::Printf(TEXT("Class : %s,Function: %s,Quest is not active: QuestTag: %s"),
				  *GetName(),*FString(__FUNCTION__),
				  *QuestTag.ToString()));
		return {};
	}
	else return ActiveQuests[QuestTag].CurrentTarget;
}

int AThirdPersonPlayerController::GetQuestStageInt(const FGameplayTag& QuestTag)
{
	if (!IsQuestActive(QuestTag))
		return 0;
	else return ActiveQuests[QuestTag].CurrentTarget.StageInt;
}

inline bool AThirdPersonPlayerController::IsQuestFocused(const FGameplayTag& QuestTag)
{
	return FocusedQuest == QuestTag;
}

bool AThirdPersonPlayerController::HasFocusedQuest()
{
	return FocusedQuest.IsValid();
}

void AThirdPersonPlayerController::AddActiveQuest_Implementation(const FGameplayTag& QuestTag)
{
	auto Quest = QuestSubsystem->GetQuest(QuestTag);
	FS_PlayerQuestHandler HoldQuest;
	HoldQuest.CurrentTarget.StageInt = Quest.Targets[1].StageInt;
	HoldQuest.CurrentTarget.IsCompleted = false;
	HoldQuest.CurrentTarget.TargetDescription = Quest.Targets[1].TargetDescription;
	HoldQuest.CurrentTarget.TargetName = Quest.Targets[1].TargetName;
	HoldQuest.IsFinished = false;
	ActiveQuests.Add(QuestTag,HoldQuest);
}

void AThirdPersonPlayerController::RemoveActiveQuest(const FGameplayTag& QuestTag)
{
	ActiveQuests.Remove(QuestTag);
}

void AThirdPersonPlayerController::GotoNextStage_Implementation(const FGameplayTag& QuestTag)
{
	if (!IsQuestActive(QuestTag))
	{
		GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
			   FString::Printf(TEXT("Class : %s,Function: %s,Quest is not active: QuestTag: %s"),
				  *GetName(),*FString(__FUNCTION__),
				  *QuestTag.ToString()));
		return;
	}
	auto Quest = QuestSubsystem->GetQuest(QuestTag);
	auto &HoldQuest = ActiveQuests[QuestTag];
	for (int i = 0; i < Quest.Targets.Num(); ++i)
	{
		if (Quest.Targets[i].StageInt == HoldQuest.CurrentTarget.StageInt)
		{
			//正常推进任务
			if (i+1 < Quest.Targets.Num())
			{
				HoldQuest.CurrentTarget.StageInt = Quest.Targets[i+1].StageInt;
				HoldQuest.CurrentTarget.TargetDescription = Quest.Targets[i+1].TargetDescription;
				HoldQuest.CurrentTarget.TargetName = Quest.Targets[i+1].TargetName;
				HoldQuest.CurrentTarget.IsCompleted = false;
			}
			//任务结束
			else
			{
				QuestFinished(QuestTag);
			}
			break;
		}
	}
}

bool AThirdPersonPlayerController::SetFocusedQuest_Implementation(const FGameplayTag& QuestTag)
{
	if (IsQuestActive(QuestTag))
	{
		FocusedQuest = QuestTag;
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
		   FString::Printf(TEXT("Class : %s,Function: %s,Quest is not active: QuestTag: %s"),
			  *GetName(),*FString(__FUNCTION__),
			  *QuestTag.ToString()));
		return false;
	}
}
























