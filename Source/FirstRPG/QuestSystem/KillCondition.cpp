// Fill out your copyright notice in the Description page of Project Settings.


#include "KillCondition.h"
UE_DEFINE_GAMEPLAY_TAG(KillConditionTag, "Action.Kill")

UKillCondition::UKillCondition()
{
	this->_Type = E_QuestTargetConditionType::Kill;
}

bool UKillCondition::ConditionPassed(const FS_QuestTargetData& QuestTargetData)
{
	checkf(QuestTargetData.ActionTag == KillConditionTag,
		TEXT("Class: %s,Action tag should be %s"),*GetName(),*KillConditionTag.GetTag().ToString());
	if (_RequiredCount <= 0) return true;
	if (QuestTargetData.TargetTag == _TargetTag)
	{
		Lock.Lock();
		_RequiredCount -= QuestTargetData.TargetCnt;
	}
	if (_RequiredCount <= 0) return true;
	return false;
}
