// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GeneralActor.h"
#include "ItemManagerSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTRPG_API UItemManagerSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	FCriticalSection IDMutex;
	TMap<FString,AGeneralActor*> ID2ItemMap;
	TMap<FString,FString> Name2IDMap;
	TMap<FGameplayTag,FString> Tag2IDMap;
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable,Category= "Item Mannger")
	FString GenerateID(AGeneralActor * Item);
	
	UFUNCTION(BlueprintCallable,BlueprintPure,Category= "Item Mannger")
	inline FString GetItemID(const FString& ItemName) const
	{
		if (!Name2IDMap.Contains(ItemName))
		{
			GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
				FString::Printf(TEXT("Class: %s, Function: %s,Cannot find Item: Name : %s"),
					*GetClass()->GetName(),*FString(__FUNCTION__),
					*ItemName));
			return "";
		}
		return Name2IDMap[ItemName];
	}
	
	UFUNCTION(BlueprintCallable,Category= "Item Mannger")
	inline AGeneralActor* GetItemByID(const FString& ID) const
	{
		if (!ID2ItemMap.Contains(ID))
		{
			GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
				FString::Printf(TEXT("Class : %s, Cannot find Item: ID : %s"),
					*GetClass()->GetName(),*ID));
			return nullptr;
		}
		return ID2ItemMap[ID];
	}
	
	UFUNCTION(BlueprintCallable,Category= "Item Mannger")
	inline AGeneralActor * GetItemByName(const FString& ItemName) const
	{
		auto ID = GetItemID(ItemName);
		return GetItemByID(ID);
	}
	
	//利用Tag来获取物品，唯一性由你自己设计的Tag保证。
	UFUNCTION(BlueprintCallable,Category= "Item Mannger")
	inline AGeneralActor* GetItemByTag(const FGameplayTag& ItemTag) const;
};
