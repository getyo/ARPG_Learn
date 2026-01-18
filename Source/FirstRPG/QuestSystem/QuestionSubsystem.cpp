// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestionSubsystem.h"
#include "QuestTargetCondition.h"
#include  "GlobalQuestTargetMessenger.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "FirstRPG/Character//ThirdPersonPlayerController.h"
#include "Misc/TextFilterExpressionEvaluator.h"

void UQuestionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UQuestTargetCondition::InitQuestTargetCondition();
	ReadQuestion();
	//初始化任务目标完成事件分发器
	auto GI = GetWorld()->GetGameInstance();
	auto QuestTargetMessenger = GI->GetSubsystem<UGlobalQuestTargetMessenger>();
	if (!QuestTargetMessenger)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f,FColor::Red,
			FString::Printf(TEXT("Class: %s,Cannot find QuestTargetMessenger"),
				*GetName()));
	}
	else
	{
		QuestTargetMessenger->OnQuestTargetFinishedEvent.AddDynamic(this,&UQuestionSubsystem::DeliverTargetCheck);
	}
}

void UQuestionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UQuestionSubsystem::ReadQuestion()
{
	_QuestInfos.Reset();
	if (DataTableRef) {
		TArray<FName> RowNames = DataTableRef->GetRowNames();
		for (auto Name : RowNames) {
			FS_QuestInfo Question = *DataTableRef->FindRow<FS_QuestInfo>(
				Name,
				FString::Printf(TEXT("Cann't Find Row :%s"), *Name.ToString()),
				true
			);
			_QuestInfos.Add(Name.ToString(),Question);
		}
		
		//每个任务的每个目标，都根据其条件描述数组生成对应条件实例
		for (auto& Quest : _QuestInfos)
		{
			for (auto &Target : Quest.Value.Targets)
			{
				for (auto &ConditionDescription : Target.EditedQuestTargetConditions)
				{
					Target.Conditions.Add(UQuestTargetCondition::QuestTargetConditionFactory(ConditionDescription.Type,
						ConditionDescription.TargetTag,ConditionDescription.RequiredCount,this));
				}
			}
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No  Data Table!"));
	}
}

FS_QuestInfo UQuestionSubsystem::GetQuest(const FString& QuestID)
{
		
	auto Quest = _QuestInfos.Find(QuestID);
	if (Quest == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, 
			FString::Printf(TEXT("[%s] 无法查找到任务: %s"), *GetName(), *QuestID)	
		);
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, 
		FString::Printf(TEXT("调用者: %s | 错误ID: [%s]"), *GetOuter()->GetName(), *QuestID)    
		);
		return FS_QuestInfo();
	}
	else
	{
		return *Quest;
	}
}

TArray<FString> UQuestionSubsystem::GetAllQuestID()
{
	TArray<FString> QuestIDs;
	_QuestInfos.GenerateKeyArray(QuestIDs);
	return QuestIDs;
}

FS_QuestTarget UQuestionSubsystem::GetQeustTarget(const FString& QuestID, int Stage)
{
	auto Quest = _QuestInfos.Find(QuestID);
	if (Quest == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,  
		FColor::Red,
		FString::Printf(TEXT("Class: %s,Cannot find Question,QuestID:%s"), *GetClass()->GetName(),*QuestID)
		);
		return FS_QuestTarget();
	}
	for (auto Element : Quest->Targets)
	{
		if (Element.StageInt == Stage)
			return Element;
	}
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,
FString::Printf(TEXT("Class: %s,Cannot find Stage,QuestID:%s,Stage:%d"), *GetClass()->GetName(),*QuestID,Stage)
	);
	return FS_QuestTarget();
}

FS_QuestTarget UQuestionSubsystem::GetNextQeustTarget(const FString& QuestID, int Stage)
{
	auto Quest = _QuestInfos.Find(QuestID);
	if (Quest == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,  
		FColor::Red,
		FString::Printf(TEXT("Class: %s,Cannot find Question,QuestID:%s"), *GetClass()->GetName(),*QuestID)
		);
		return FS_QuestTarget();
	}
	for (int i = 0; i < Quest->Targets.Num(); ++i)
	{
		if (Quest->Targets[i].StageInt == Stage && (i+1) < Quest->Targets.Num())
			return Quest->Targets[i+1];
	}
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,
FString::Printf(TEXT("Class: %s,Cannot find Next Stage,QuestID:%s,Stage:%d"), *GetClass()->GetName(),*QuestID,Stage)
	);
	return FS_QuestTarget(-1);
}

void UQuestionSubsystem::DeliverTargetCheck(const FS_QuestTargetData& QuestTargetData)
{
	auto PlayerController = Cast<AThirdPersonPlayerController> (QuestTargetData.Instigator);
	if (PlayerController)
	{
		auto ActiveQuest = PlayerController->GetAllActiveQuest();
		for (auto Quest : ActiveQuest)
		{
			auto QuestTargetConditions = GetQeustTarget(Quest.Key,Quest.Value.CurrentTarget.StageInt).Conditions;
			for (auto Condition : QuestTargetConditions)
			{
				if (Condition->GetQuestTargetConditionType() == QuestTargetData.Type)
				{
					if (Condition->ConditionPassed(QuestTargetData))
					{
						if (!PlayerController->GotoNextStage(Quest.Key))
						{
							PlayerController->CompleteQuest(Quest.Key);
						}
					}
				}
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f,FColor::Red,
			FString::Printf(TEXT("Class: %s, FS_QuestTargetData Instigator cannot cast to ThridPersonPlayerController"),
			*GetName()));
		
	}

}

void UQuestionSubsystem::BroadcastFinish(FS_QuestTargetData QuestTargetData)
{
	auto GI = GetWorld()->GetGameInstance();
	auto QuestTargetMessenger = GI->GetSubsystem<UGlobalQuestTargetMessenger>();
	if (!QuestTargetMessenger)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f,FColor::Red,
			FString::Printf(TEXT("Class: %s,Cannot find QuestTargetMessenger"),
				*GetName()));
	}
	else
	{
		QuestTargetMessenger->OnQuestTargetFinishedEvent.Broadcast(QuestTargetData);
	}
}


















