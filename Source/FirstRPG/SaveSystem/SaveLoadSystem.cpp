// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveLoadSystem.h"
#include "FirstRPG/Debug/Debug.h"
#include "SaveGameContainer.h"
#include "FirstRPG/Character/Player/ThirdPersonPlayerController.h"
#include "Kismet/GameplayStatics.h"

void USaveLoadSystem::SpawnActors()
{
	// 物体生成
	for (auto it = ItemsData.begin(); it != ItemsData.end(); ++it)
	{
		const auto& ItemData = it->Value; // 直接获取结构体引用
		if (!ItemData.IsStatic && !ItemData.Dead)
		{
			if (ItemData.ItemClass && ItemData.ItemClass->IsChildOf(AEquippableItemActor::StaticClass()))
			{
				auto Inst = UEquipmentInstance::EquipmentInstanceFactory(ItemData.ItemInfoRowHandler);
				FTransform Transform;
				Transform.SetLocation(ItemData.Location);
				Transform.SetRotation(ItemData.Rotation.Quaternion());
				Transform.SetScale3D(ItemData.Scale);
				auto ItemObj = AGeneralItemActor::SpawnItem(GetWorld(), ItemData.ItemClass, Inst, Transform);
				ItemObjRef.Add(it->Key, ItemObj);
			}
			else
			{
				auto Inst = UGeneralItemInstance::GeneralItemInstanceFactory(ItemData.ItemInfoRowHandler);
				FTransform Transform;
				Transform.SetLocation(ItemData.Location);
				Transform.SetRotation(ItemData.Rotation.Quaternion());
				Transform.SetScale3D(ItemData.Scale);
				auto ItemObj = AGeneralItemActor::SpawnItem(GetWorld(), ItemData.ItemClass, Inst, Transform);
				ItemObjRef.Add(it->Key, ItemObj);
			}
		}
		else if (ItemData.Dead)
		{
			if (auto ItemObjPtr = ItemObjRef.Find(it->Key))
			{
				if (auto* ItemObj = *ItemObjPtr)
				{
					ItemObj->Destroy();
					ItemObjRef[it->Key] = nullptr;
				}
				else
				{
					CPP_LOG(Warning, "Character Object is invalid! Name"+ItemData.Name);
				}
			}
			else
			{
				CPP_LOG(Warning, "Static Character Object not found! Name:"+ItemData.Name);
			}
			
		}
	}
	
	// 角色生成
	for (auto it = CharacterData.begin(); it != CharacterData.end(); ++it)
	{
		const auto& CharData = it->Value; // 直接获取结构体引用
		if (!CharData.IsStatic && !CharData.Dead)
		{
			FTransform Transform;
			Transform.SetLocation(CharData.Location);
			Transform.SetRotation(CharData.Rotation.Quaternion());
			Transform.SetScale3D(CharData.Scale);
			auto CharacterObj = AGeneralCharacter::SpawnCharacter(GetWorld(), CharData.CharacterClass, Transform);
			CharacterObjRef.Add(it->Key, CharacterObj);
		}
		else if (CharData.Dead)
		{
			if (auto CharacterObjPtr = CharacterObjRef.Find(it->Key))
			{
				if (auto* CharacterObj = *CharacterObjPtr)
				{
					CharacterObj->Destroy();
					CharacterObjRef[it->Key] = nullptr;
				}
				else
				{
					CPP_LOG(Warning, "Character Object is invalid! Name"+CharData.Name);
				}
			}
			else
			{
				CPP_LOG(Warning, "Static Character Object not found! Name:"+CharData.Name);
			}
			
		}
	}
}

void USaveLoadSystem::LoadGame()
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CPP_LOG(Error, "Save file does not exist! " + SlotName);
		return;
	}

	USaveGameContainer* SaveGameInstance = Cast<USaveGameContainer>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (!SaveGameInstance) return;

	// 拷贝数据到子系统变量
	this->ItemsData = SaveGameInstance->ItemsData;
	this->CharacterData = SaveGameInstance->CharacterData;
	this->PlayerData = SaveGameInstance->PlayerData;
	
	SpawnActors();
	
	for (auto Pair : ItemsData)
	{
		auto ID = Pair.Key;
		auto Data = Pair.Value;
		if (auto* FoundPtr = ItemObjRef.Find(ID))
		{
			if (auto* ItemObj = *FoundPtr)
			{
				ItemObj->Execute_LoadItemData(ItemObj,Data);
			}
		}
		else
		{
			CPP_LOG(Error, "Cannot Get ItemObj: " + Data.Name);
		}
	}
	
	for (auto Pair : CharacterData)
	{
		auto ID = Pair.Key;
		auto Data = Pair.Value;
		if (auto* FoundPtr = CharacterObjRef.Find(ID))
		{
			if (auto* CharacterObj = *FoundPtr)
			{
				CharacterObj->Execute_LoadCharacterData(CharacterObj,Data);
			}
		}
		else
		{
			CPP_LOG(Error, "Cannot Get CharacterObj: " + Data.Name);
		}
	}
	
	// 玩家数据的注入
	auto PlayerCtrl= Cast<AThirdPersonPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PlayerCtrl)
	{
		CPP_LOG(Error, "Player Controller not found!");
		return;
	}
	PlayerCtrl->Execute_LoadPlayerData(PlayerCtrl,PlayerData);
}

void USaveLoadSystem::SaveGame()
{
	USaveGameContainer* SaveGameInstance = Cast<USaveGameContainer>(UGameplayStatics::CreateSaveGameObject(USaveGameContainer::StaticClass()));
	if (!SaveGameInstance) return;

	// 2. 收集所有物品 Actor 的数据（使用栈分配的结构体，替代 NewObject）
	for (auto& Pair : ItemObjRef)
	{
		FString ID = Pair.Key;
		AGeneralItemActor* ItemActor = Pair.Value;
		if (!ItemActor)
		{
			FItemSaveData ItemData;
			ItemData.Dead = true;
			SaveGameInstance->ItemsData.Add(ID, ItemData);
			continue;
		}
		
		FItemSaveData ItemData;
		ItemActor->Execute_SaveItemData(ItemActor,ItemData);
		SaveGameInstance->ItemsData.Add(ID, ItemData);
	}

	// 3. 收集所有角色 Actor 的数据
	for (auto& Pair : CharacterObjRef)
	{
		FString ID = Pair.Key;
		AGeneralCharacter* CharActor = Pair.Value;
		if (!CharActor)
		{
			FCharacterSaveData CharData;
			CharData.Dead = true;
			SaveGameInstance->CharacterData.Add(ID, CharData);
			continue;
		}
		if (CharActor->ActorHasTag("Player")) continue;
		FCharacterSaveData CharData;
		CharActor->Execute_SaveCharacterData(CharActor,CharData);
		
		SaveGameInstance->CharacterData.Add(ID, CharData);
	}
	
	auto PlayerCtrl= Cast<AThirdPersonPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PlayerCtrl)
	{
		CPP_LOG(Error, "Player Controller not found!");
		return;
	}
	PlayerCtrl->Execute_SavePlayerData(PlayerCtrl,PlayerData);
	SaveGameInstance->PlayerData = PlayerData;
	// 4. 将整个包裹写入硬盘
	bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
	
	if (bSuccess)
	{
		CPP_LOG(Verbose, "Game Saved Successfully!");
	}
}

void USaveLoadSystem::DeleteSaveData()
{
	bool bSuccess = UGameplayStatics::DeleteGameInSlot(SlotName, 0);

	if (bSuccess)
	{
		CPP_LOG(Verbose, "Save file deleted successfully!");
	}
	else
	{
		CPP_LOG(Warning, "Failed to delete save file or file does not exist.");
	}
}

void USaveLoadSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USaveLoadSystem::Deinitialize()
{
	Super::Deinitialize();
}

void USaveLoadSystem::Register(AActor* Obj)
{
	if (Obj == nullptr) return;
	if (auto Character = Cast<AGeneralCharacter>(Obj))
		CharacterObjRef.Add(Character->GetCharacterID(), Character);
	else if (auto Item = Cast<AGeneralItemActor>(Obj))
		ItemObjRef.Add(Item->GetItemID(), Item);
}

void USaveLoadSystem::UnRegister(AActor* Obj)
{
	if (Obj == nullptr) return;
	if (auto Character = Cast<AGeneralCharacter>(Obj))
	{
		if (CharacterObjRef.Find(Character->GetCharacterID()))
		{
			CharacterObjRef[Character->GetCharacterID()] = nullptr;
		}
	}
		
	else if (auto Item = Cast<AGeneralItemActor>(Obj))
	{
		if (ItemObjRef.Find(Item->GetItemID()))
		{
			ItemObjRef[Item->GetItemID()] = nullptr;
		}
	}
}
