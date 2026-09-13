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
				if (ItemObj) ItemObjRef.Add(it->Key, ItemObj);
				else CPP_LOG(Warning, FString::Printf(TEXT("SpawnItem failed: Class=%s, Key=%s, Name=%s"),
					ItemData.ItemClass ? *ItemData.ItemClass->GetName() : TEXT("NULL"), *it->Key, *ItemData.Name));
			}
			else
			{
				auto Inst = UGeneralItemInstance::GeneralItemInstanceFactory(ItemData.ItemInfoRowHandler);
				FTransform Transform;
				Transform.SetLocation(ItemData.Location);
				Transform.SetRotation(ItemData.Rotation.Quaternion());
				Transform.SetScale3D(ItemData.Scale);
				auto ItemObj = AGeneralItemActor::SpawnItem(GetWorld(), ItemData.ItemClass, Inst, Transform);
				if (ItemObj) ItemObjRef.Add(it->Key, ItemObj);
				else CPP_LOG(Warning, FString::Printf(TEXT("SpawnItem failed: Class=%s, Key=%s, Name=%s"),
					ItemData.ItemClass ? *ItemData.ItemClass->GetName() : TEXT("NULL"), *it->Key, *ItemData.Name));
			}
		}
		else if (ItemData.Dead)
		{
			if (auto ItemObjPtr = ItemObjRef.Find(it->Key))
			{
				if (auto* ItemObj = *ItemObjPtr)
				{
					ItemObj->Destroy();
					ItemObjRef.Remove(it->Key);
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
			// 生成失败不能把原映射覆盖成空（否则下次会被误写成 Dead=true，再下次读档就 Destroy 掉还活着的对象）
			if (CharacterObj) CharacterObjRef.Add(it->Key, CharacterObj);
			else CPP_LOG(Warning, FString::Printf(TEXT("SpawnCharacter failed: Class=%s, Key=%s, Name=%s"),
				CharData.CharacterClass ? *CharData.CharacterClass->GetName() : TEXT("NULL"), *it->Key, *CharData.Name));
		}
		else if (CharData.Dead)
		{
			if (auto CharacterObjPtr = CharacterObjRef.Find(it->Key))
			{
				if (auto* CharacterObj = *CharacterObjPtr)
				{
					CharacterObj->Destroy();
					CharacterObjRef.Remove(it->Key);
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

	if (SaveGameInstance->SaveVersion < 1)
	{
		CPP_LOG(Warning, "Loading legacy save (version < 1): quest data may be missing.");
	}

	// 拷贝数据到子系统变量
	this->ItemsData = SaveGameInstance->ItemsData;
	this->CharacterData = SaveGameInstance->CharacterData;
	this->PlayerData = SaveGameInstance->PlayerData;
	
	// 死亡记录重新积累（读档时会根据 Dead 分支重新填充）
	DeadItemKeys.Reset();
	DeadCharacterKeys.Reset();
	
	SpawnActors();
	
	for (auto Pair : ItemsData)
	{
		auto ID = Pair.Key;
		auto Data = Pair.Value;
		// “死后不回填”的条目（空位）直接跳过，否则会刷 Cannot Get ItemObj
		if (Data.Dead || ID.IsEmpty()) continue;
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
		// “死后不回填”的条目（空位）直接跳过
		if (Data.Dead || ID.IsEmpty()) continue;
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
	
	// 3.5 显式死亡记录：已注销、且不在注册表里的 key，写成 Dead=true
	for (const FString& Key : DeadItemKeys)
	{
		if (!ItemObjRef.Contains(Key))
		{
			FItemSaveData ItemData;
			ItemData.Dead = true;
			SaveGameInstance->ItemsData.Add(Key, ItemData);
		}
	}
	for (const FString& Key : DeadCharacterKeys)
	{
		if (!CharacterObjRef.Contains(Key))
		{
			FCharacterSaveData CharData;
			CharData.Dead = true;
			SaveGameInstance->CharacterData.Add(Key, CharData);
		}
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
	SaveGameInstance->SaveVersion = 1;
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
	{
		CharacterObjRef.Add(Character->GetCharacterID(), Character);
		DeadCharacterKeys.Remove(Character->GetCharacterID());
	}
	else if (auto Item = Cast<AGeneralItemActor>(Obj))
	{
		ItemObjRef.Add(Item->GetItemID(), Item);
		DeadItemKeys.Remove(Item->GetItemID());
	}
}

void USaveLoadSystem::UnRegister(AActor* Obj)
{
	if (Obj == nullptr) return;
	if (auto Character = Cast<AGeneralCharacter>(Obj))
	{
		if (CharacterObjRef.Contains(Character->GetCharacterID()))
		{
			// 不再置空：移出注册表，并记到显式死亡集合
			CharacterObjRef.Remove(Character->GetCharacterID());
			DeadCharacterKeys.Add(Character->GetCharacterID());
		}
	}
		
	else if (auto Item = Cast<AGeneralItemActor>(Obj))
	{
		if (ItemObjRef.Contains(Item->GetItemID()))
		{
			ItemObjRef.Remove(Item->GetItemID());
			DeadItemKeys.Add(Item->GetItemID());
		}
	}
}
