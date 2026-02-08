// Fill out your copyright notice in the Description page of Project Settings.


#include "KillCondition.h"
UE_DEFINE_GAMEPLAY_TAG(KillConditionTag, "Quest.Action.Kill")

UKillCondition::UKillCondition()
{
	this->_Type = KillConditionTag;
}

bool UKillCondition::ConditionPassed(const FS_QuestTargetData& QuestTargetData)
{
	checkf(QuestTargetData.ActionTag == KillConditionTag,
		TEXT("Class: %s,Function:%s,Action tag is %s,it should be %s"),
		*GetName(),*FString(__FUNCTION__),
		*QuestTargetData.ActionTag.ToString(),*KillConditionTag.GetTag().ToString());
	if (_RequiredCount <= 0) HasPassed = true;
	else if (QuestTargetData.TargetTag == _TargetTag)
	{
		Lock.Lock();
		_RequiredCount -= QuestTargetData.TargetCnt;
	}
	if (_RequiredCount <= 0) HasPassed =  true;
	return HasPassed;
}
