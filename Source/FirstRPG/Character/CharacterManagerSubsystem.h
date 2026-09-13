// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NativeGameplayTags.h"
#include "CharacterManagerSubsystem.generated.h"


class AGeneralItemActor;
/**
 * 
 */
UCLASS(BlueprintType)
class FIRSTRPG_API UCharacterManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	FCriticalSection IDMutex;
	TMap<FString,AGeneralCharacter*> ID2CharcCharacterMap;
	TMap<FString,FString> Name2IDMap;
	TMap<FGameplayTag,FString> Tag2IDMap;
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable,Category= "Character Mannger")
	void Register(AGeneralCharacter* Character);
	void UnRegister(AGeneralCharacter* Character);
	
	UFUNCTION(BlueprintCallable,BlueprintPure,Category= "Character Mannger")
	inline FString GetCharacterID(const FString& CharacterName) const
	{
		if (!Name2IDMap.Contains(CharacterName))
		{
			GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
				FString::Printf(TEXT("Class: %s,Function: %s, Cannot find Character: Name : %s"),
					*GetClass()->GetName(),*FString(__FUNCTION__),
					*CharacterName));
			return "";
		}
		return Name2IDMap[CharacterName];
		
	}
	
	UFUNCTION(BlueprintCallable,Category= "Character Mannger")
	inline AGeneralCharacter* GetCharacterByID(const FString& ID) const
	{
		if (!ID2CharcCharacterMap.Contains(ID))
		{
			GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
				FString::Printf(TEXT("Class : %s, Cannot find Character: ID : %s"),
					*GetClass()->GetName(),*ID));
			return nullptr;
		}
		return ID2CharcCharacterMap[ID];
	}
	
	UFUNCTION(BlueprintCallable,Category= "Character Mannger")
	inline AGeneralCharacter* GetCharacterByName(const FString& Character) const
	{
		auto ID = GetCharacterID(Character);
		return GetCharacterByID(ID);
	}
	//利用Tag来获取角色，唯一性由你自己设计的Tag保证。
	UFUNCTION(BlueprintCallable,Category= "Character Mannger")
	inline AGeneralCharacter* GetCharacterByTag(const FGameplayTag& CharacterTag) const;
};
