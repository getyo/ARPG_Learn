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
	//因为对话有其特殊性，你的对话资产本身就存在任务阶段+参与对象的信息，
	//既然能找到这里就必定是正确的节点，保留第二个条件是为了兼容和以防万一
	if (_TargetTag.ToString().IsEmpty() ||
		QuestTargetData.TargetTag == _TargetTag )
		HasPassed = true;
	else HasPassed = false;
	return HasPassed;
}
