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

























