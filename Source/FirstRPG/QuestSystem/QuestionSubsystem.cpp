// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestionSubsystem.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "QuestTargetCondition.h"
#include "GlobalQuestTargetMessenger.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "FirstRPG/Character//ThirdPersonPlayerController.h"

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
	if (PlayerController)
	{
		bool ActiveTag = false;
		FS_QuestInfo * QuestPtr = nullptr;
		if (!_QuestInfos.Find(QuestTargetData.QuestID))
		{
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,
	FString::Printf(TEXT("Class: %s,Function : %s,Cannot find Quest,QuestID:%s"), 
				*GetClass()->GetName(),*FString(__FUNCTION__),*QuestTargetData.QuestID));
			return;
		}
		else QuestPtr = &_QuestInfos[QuestTargetData.QuestID];
		//如果是触发了某个任务的开始条件，则把它加入激活任务
		bool AllPassed = true;
		if (!QuestTargetData.Stage)
		{
			ActiveTag = true;
			for (auto &Condition : QuestPtr->Targets[0].Conditions)
			{
				if (Condition->GetQuestTargetConditionType() == QuestTargetData.Type)
				{
					if (!Condition->ConditionPassed(QuestTargetData))
					{
						AllPassed = false;
						return;
					}
				}
				else AllPassed = Condition->GetPassed() && AllPassed;
			}
			if (AllPassed)
				PlayerController->AddActiveQuest(QuestTargetData.QuestID);
		}
		if (ActiveTag) return;
		//正常的任务推进
		for (auto &Target : QuestPtr->Targets)
		{
			if (Target.StageInt == QuestTargetData.Stage)
			{
				for (auto &Condition : Target.Conditions)
				{
					if (Condition->GetQuestTargetConditionType() == QuestTargetData.Type)
					{
						if (!Condition->ConditionPassed(QuestTargetData))
						{
							AllPassed = false;
							return;
						}
					}
					else AllPassed = Condition->GetPassed() && AllPassed;
				}
				if (AllPassed)
					PlayerController->GotoNextStage(QuestTargetData.QuestID);
			}
		}
	}
	else
	{
		PlayerController = Cast<AThirdPersonPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
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


















