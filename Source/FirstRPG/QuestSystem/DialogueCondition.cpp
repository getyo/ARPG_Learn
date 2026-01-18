// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueCondition.h"
UE_DEFINE_GAMEPLAY_TAG(TalkConditionTag, "Action.Talk")

UDialogueCondition::UDialogueCondition()
{
	this->_Type = E_QuestTargetConditionType::Talk;
}

bool UDialogueCondition::ConditionPassed(const FS_QuestTargetData& QuestTargetData)
{
	checkf(QuestTargetData.ActionTag == TalkConditionTag,
		TEXT("Class: %s,Action tag should be %s"),
		*GetName(),*TalkConditionTag.GetTag().ToString());
	if (QuestTargetData.TargetTag == _TargetTag)
		return true;
	else return false;
}
