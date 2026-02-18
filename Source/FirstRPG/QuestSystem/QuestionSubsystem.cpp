// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestionSubsystem.h"
#include "QuestTargetCondition.h"
#include "GlobalQuestTargetMessenger.h"
#include "QuestDeveloperSettings.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "FirstRPG/Character/Player/ThirdPersonPlayerController.h"

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
	auto QuestID2TagMap = UQuestDeveloperSettings::Get()->GetQuesID2TagMap();
	_QuestInfos.Reset();
	if (DataTableRef) {
		TArray<FName> RowNames = DataTableRef->GetRowNames();
		for (auto Name : RowNames) {
			FS_QuestInfo Question = *DataTableRef->FindRow<FS_QuestInfo>(
				Name,
				FString::Printf(TEXT("Cann't Find Row :%s"), *Name.ToString()),
				true
			);
			auto QuestTagPtr = QuestID2TagMap->Find(Name.ToString());
			checkf(QuestTagPtr,
			TEXT("Class: %s,Function:%s,Cannot Find QuestID map to Tag: QuestID: %s"),
			*GetName(),*FString(__FUNCTION__),
			*Name.ToString());
			_QuestInfos.Add(*QuestTagPtr,Question);
		}
		
		//每个任务的每个目标，都根据其条件描述数组生成对应条件实例
		for (auto& Quest : _QuestInfos)
		{
			for (auto &Target : Quest.Value.Targets)
			{
				for (auto &ConditionDescription : Target.EditedQuestTargetConditions)
				{
					Target.Conditions.Add(UQuestTargetCondition::QuestTargetConditionFactory(
						ConditionDescription.ActionTag,ConditionDescription.TargetTag,ConditionDescription.RequiredCount,this));
				}
			}
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No  Data Table!"));
	}
}

FS_QuestInfo UQuestionSubsystem::GetQuest(const FGameplayTag& QuestTag)
{
		
	auto Quest = _QuestInfos.Find(QuestTag);
	if (Quest == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, 
			FString::Printf(TEXT("[%s] 无法查找到任务: %s"), *GetName(), *QuestTag.ToString())	
		);
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, 
		FString::Printf(TEXT("调用者: %s | 错误ID: [%s]"), *GetOuter()->GetName(), *QuestTag.ToString())    
		);
		return FS_QuestInfo();
	}
	else
	{
		return *Quest;
	}
}

TArray<FGameplayTag> UQuestionSubsystem::GetAllQuestID()
{
	TArray<FGameplayTag> QuestIDs;
	_QuestInfos.GenerateKeyArray(QuestIDs);
	return QuestIDs;
}

FS_QuestTarget UQuestionSubsystem::GetQuestTarget(const FGameplayTag& QuestTag, int Stage)
{
	auto Quest = _QuestInfos.Find(QuestTag);
	if (Quest == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,  
		FColor::Red,
		FString::Printf(TEXT("Class: %s,Cannot find Question,QuestID:%s"), *GetClass()->GetName(),*QuestTag.ToString())
		);
		return FS_QuestTarget();
	}
	for (auto Element : Quest->Targets)
	{
		if (Element.StageInt == Stage)
			return Element;
	}
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,
FString::Printf(TEXT("Class: %s,Function: %s,Cannot find Stage,QuestID:%s,Stage:%d"), 
			*GetClass()->GetName(),*FString(__FUNCTION__),
			*QuestTag.ToString(),Stage)
	);
	return FS_QuestTarget();
}

FS_QuestTarget UQuestionSubsystem::GetNextQuestTarget(const FGameplayTag& QuestTag, int Stage)
{
	auto Quest = _QuestInfos.Find(QuestTag);
	if (Quest == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,  
		FColor::Red,
		FString::Printf(TEXT("Class: %s,Cannot find Question,QuestID:%s"), *GetClass()->GetName(),*QuestTag.ToString())
		);
		return FS_QuestTarget();
	}
	for (int i = 0; i < Quest->Targets.Num(); ++i)
	{
		if (Quest->Targets[i].StageInt == Stage && (i+1) < Quest->Targets.Num())
			return Quest->Targets[i+1];
	}
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,
FString::Printf(TEXT("Class: %s,Cannot find Next Stage,QuestID:%s,Stage:%d"), *GetClass()->GetName(),*QuestTag.ToString(),Stage)
	);
	return FS_QuestTarget(-1);
}

void UQuestionSubsystem::DeliverTargetCheck(const FS_QuestTargetData& QuestTargetDescription)
{
	if (PlayerController)
	{
		bool ActiveTag = false;
		FS_QuestInfo * QuestPtr = nullptr;
		if (!_QuestInfos.Find(QuestTargetDescription.QuestTag))
		{
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,
	FString::Printf(TEXT("Class: %s,Function : %s,Cannot find Quest,QuestID:%s"), 
				*GetClass()->GetName(),*FString(__FUNCTION__),*QuestTargetDescription.QuestTag.ToString()));
			return;
		}
		else QuestPtr = &_QuestInfos[QuestTargetDescription.QuestTag];
		bool AllPassed = true;
		//如果是触发了某个任务的开始条件，则把它加入激活任务
		if (!QuestTargetDescription.Stage)
		{
			//如果条件为空，说明只能通过接取按钮激活
			if (QuestPtr->Targets[0].Conditions.IsEmpty())
				return;
			ActiveTag = true;
			for (auto &Condition : QuestPtr->Targets[0].Conditions)
			{
				if (Condition->GetQuestTargetConditionType() == QuestTargetDescription.ActionTag)
				{
					if (!Condition->ConditionPassed(QuestTargetDescription))
					{
						AllPassed = false;
						return;
					}
				}
				else AllPassed = Condition->GetPassed() && AllPassed;
			}
			if (AllPassed)
				PlayerController->AddActiveQuest(QuestTargetDescription.QuestTag);
		}
		if (ActiveTag) return;
		
		//正常的任务推进
		for (auto &Target : QuestPtr->Targets)
		{
			if (Target.StageInt == QuestTargetDescription.Stage)
			{
				for (auto &Condition : Target.Conditions)
				{
					if (Condition->GetQuestTargetConditionType() == QuestTargetDescription.ActionTag)
					{
						if (!Condition->ConditionPassed(QuestTargetDescription))
						{
							AllPassed = false;
							return;
						}
					}
					else AllPassed = Condition->GetPassed() && AllPassed;
				}
				if (AllPassed)
					PlayerController->GotoNextStage(QuestTargetDescription.QuestTag);
			}
		}
	}
	else
	{
		PlayerController = Cast<AThirdPersonPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		DeliverTargetCheck(QuestTargetDescription);
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


















