// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTargetCondition.h"
#include "QuestDeveloperSettings.h"
const TMap<E_QuestTargetConditionType,TSubclassOf<UQuestTargetCondition>>* UQuestTargetCondition::_E2Class = nullptr;

UQuestTargetCondition::UQuestTargetCondition()
{
}

void UQuestTargetCondition::InitQuestTargetCondition()
{
	_E2Class = UQuestDeveloperSettings::Get()->GetConditionMap();
}

UQuestTargetCondition* UQuestTargetCondition::QuestTargetConditionFactory(E_QuestTargetConditionType Type,FGameplayTag TargetTag,
		int RequestedNum,UObject* Outer)
{
	const TSubclassOf<UQuestTargetCondition>* ConditionClass = _E2Class->Find(Type);
	if (!ConditionClass)
	{
		const UEnum * EnumClass = StaticEnum<E_QuestTargetConditionType>();
		UE_LOG(LogTemp, Error, TEXT("Class: %s, Cannot find subclass of %s: %s"),
			*UQuestTargetCondition::StaticClass()->GetName(),*UQuestTargetCondition::StaticClass()->GetName(),
			*EnumClass->GetNameByValue(static_cast<int>(Type)).ToString());
		return nullptr;
	}
	auto NewCondition =  NewObject<UQuestTargetCondition>(Outer, *ConditionClass);
	NewCondition->SetDefaultProperties(TargetTag,RequestedNum,Outer);
	return NewCondition;
}

