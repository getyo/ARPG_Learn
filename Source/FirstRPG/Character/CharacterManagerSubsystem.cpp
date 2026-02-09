// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterManagerSubsystem.h"

#include "GeneralCharacter.h"


void UCharacterManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}
void UCharacterManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FString IncrementSuffix(const FString& InName)
{
	if (InName.IsEmpty()) return TEXT("1");

	int32 LastDigitIndex = InName.Len();
    
	// 1. 从后往前找，确定数字部分的起始位置
	while (LastDigitIndex > 0 && FChar::IsDigit(InName[LastDigitIndex - 1]))
	{
		LastDigitIndex--;
	}

	// 2. 提取前缀和数字后缀
	FString Prefix = InName.Left(LastDigitIndex);
	FString Suffix = InName.RightChop(LastDigitIndex);

	if (Suffix.IsEmpty())
	{
		// 如果原字符串不以数字结尾，通常做法是加个下划线和 1
		return InName + TEXT("_1");
	}
	else
	{
		// 3. 将后缀转为整数并递增
		int32 Number = FCString::Atoi(*Suffix);
		Number++;
        
		// 4. 重新拼接（保留前缀）
		return Prefix + FString::FromInt(Number);
	}
}

FString UCharacterManagerSubsystem::GenerateID(AGeneralCharacter *Character)
{
	FScopeLock Lock(&IDMutex);
	FGuid Guid = FGuid::NewGuid();
	FString ID = Guid.ToString();
	ID2CharcCharacterMap.Add(ID,Character);
	FString Name = Character->GetCharacterName();
	if (Name2IDMap.Contains(Name))
	{
		//生成一个不重复的名字
		//规则是：如果名字的结束不是以数字结尾，则加一个_1，
		//如果是，则递增数字。
		Name = IncrementSuffix(Name);
	}
	Name2IDMap.Add(Name,ID);
	Tag2IDMap.Add(Character->GetCharacterTag(),ID);
	
	return ID;
}

AGeneralCharacter* UCharacterManagerSubsystem::GetCharacterByTag(const FGameplayTag& CharacterTag) const
{
	if (Tag2IDMap.Contains(CharacterTag))
	{
		return GetCharacterByID(Tag2IDMap[CharacterTag]);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Yellow,
			FString::Printf(TEXT("Class: %s,Function: %s,Character Tag %s, does not exist."),
				*GetName(),*FString(__FUNCTION__),*CharacterTag.ToString()));
		return nullptr;
	}
}

