// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManagerSubSystem.h"
#include "GeneralActor.h"
extern FString IncrementSuffix(const FString& Name);

void UItemManagerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}
void UItemManagerSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

FString UItemManagerSubSystem::GenerateID(AGeneralActor *Item)
{
	FScopeLock Lock(&IDMutex);
	FGuid Guid = FGuid::NewGuid();
	FString ID = Guid.ToString();
	ID2ItemMap.Add(ID,Item);
	FString Name = Item->GetItemName();
	if (Name2IDMap.Contains(Name))
	{
		//生成一个不重复的名字
		//规则是：如果名字的结束不是以数字结尾，则加一个_1，
		//如果是，则递增数字。
		Name = IncrementSuffix(Name);
	}
	Name2IDMap.Add(Name,ID);
	Tag2IDMap.Add(Item->GetItemTag(),ID);
	
	return ID;
}

AGeneralActor* UItemManagerSubSystem::GetItemByTag(const FGameplayTag& ItemTag) const
{
	if (Tag2IDMap.Contains(ItemTag))
	{
		return GetItemByID(Tag2IDMap[ItemTag]);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Yellow,
			FString::Printf(TEXT("Class: %s,Function: %s,Item Tag %s, does not exist."),
				*GetName(),*FString(__FUNCTION__),*ItemTag.ToString()));
		return nullptr;
	}
}

