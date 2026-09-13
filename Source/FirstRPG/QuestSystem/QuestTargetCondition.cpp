// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTargetCondition.h"
#include "QuestDeveloperSettings.h"
const TMap<FGameplayTag,TSubclassOf<UQuestTargetCondition>>* UQuestTargetCondition::_E2Class = nullptr;

UQuestTargetCondition::UQuestTargetCondition()
{
}

void UQuestTargetCondition::InitQuestTargetCondition()
{
	_E2Class = UQuestDeveloperSettings::Get()->GetConditionMap();
}

UQuestTargetCondition* UQuestTargetCondition::QuestTargetConditionFactory(FGameplayTag ActionTag,
		FGameplayTag TargetTag,int RequestedNum,UObject* Outer)
{
	const TSubclassOf<UQuestTargetCondition>* ConditionClass = _E2Class->Find(ActionTag);
	if (!ConditionClass)
	{
		GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
								 FString::Printf(TEXT("Class: %s,Function: %s,Cannot Find ActonTag: %s"), 
												 *StaticClass()->GetName(),*FString(__FUNCTION__),
												 *ActionTag.ToString()));
		return nullptr;
	}
	auto NewCondition =  NewObject<UQuestTargetCondition>(Outer, *ConditionClass);
	NewCondition->SetDefaultProperties(TargetTag,RequestedNum,Outer);
	return NewCondition;
}

