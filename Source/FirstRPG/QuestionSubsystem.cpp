// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestionSubsystem.h"

void UQuestionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ReadQuestion();
}

void UQuestionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UQuestionSubsystem::ReadQuestion()
{
	QuestInfos.Reset();
	if (DataTableRef) {
		TArray<FName> RowNames = DataTableRef->GetRowNames();
		for (auto Name : RowNames) {
			FS_QuestInfo Question = *DataTableRef->FindRow<FS_QuestInfo>(
				Name,
				FString::Printf(TEXT("Cann't Find Row :%s"), *Name.ToString()),
				true
			);
			QuestInfos.Add(Name.ToString(),Question);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No  Data Table!"));
	}
}

FS_QuestInfo UQuestionSubsystem::GetQuest(const FString& QuestID)
{
		
	auto Quest = QuestInfos.Find(QuestID);
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
	QuestInfos.GenerateKeyArray(QuestIDs);
	return QuestIDs;
}

FS_QuestTarget UQuestionSubsystem::GetQeustTarget(const FString& QuestID, int Stage)
{
	auto Quest = QuestInfos.Find(QuestID);
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
	auto Quest = QuestInfos.Find(QuestID);
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
	return FS_QuestTarget();
}
























