// Fill out your copyright notice in the Description page of Project Settings.
#include "ThirdPersonPlayerController.h"

void AThirdPersonPlayerController::QuestFinished_Implementation(const FGameplayTag& QuestTag)
{
	ActiveQuests[QuestTag].IsFinished = true;
	FocusedQuest = FGameplayTag::EmptyTag;
	ActiveQuests.Remove(QuestTag);
}

void AThirdPersonPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UGameInstance* GI = GetWorld()->GetGameInstance();
	if (GI)
	{
		QuestSubsystem= GI->GetSubsystem<UQuestionSubsystem>();
		if (!QuestSubsystem)
		{
			GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
			   FString::Printf(TEXT("Class : %s,Function: %s,Cannot get Quest SubSystem"),
				  *GetName(),*FString(__FUNCTION__)));
		}
	}
}

TMap<FGameplayTag,FS_PlayerQuestHandler> AThirdPersonPlayerController::GetAllActiveQuest()
{
	return ActiveQuests;
}

inline bool AThirdPersonPlayerController::IsQuestActive(const FGameplayTag& QuestTag)
{
	return ActiveQuests.Contains(QuestTag);
}

inline bool AThirdPersonPlayerController::ActiveQuestEmpty()
{
	return ActiveQuests.IsEmpty();
}

inline bool AThirdPersonPlayerController::IsQuestFinished(const FGameplayTag& QuestTag)
{
	if (!IsQuestActive(QuestTag))
	{
		GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
			   FString::Printf(TEXT("Class : %s,Function: %s,Quest is not active: QuestTag: %s"),
				  *GetName(),*FString(__FUNCTION__),
				  *QuestTag.ToString()));
		return false;
	}
	return ActiveQuests[QuestTag].IsFinished;
}

FS_PlayerHoldTarget AThirdPersonPlayerController::GetQuestHoldTarget(const FGameplayTag& QuestTag)
{
	if (!IsQuestActive(QuestTag))
	{
		GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
			   FString::Printf(TEXT("Class : %s,Function: %s,Quest is not active: QuestTag: %s"),
				  *GetName(),*FString(__FUNCTION__),
				  *QuestTag.ToString()));
		return {};
	}
	else return ActiveQuests[QuestTag].CurrentTarget;
}

int AThirdPersonPlayerController::GetQuestStageInt(const FGameplayTag& QuestTag)
{
	if (!IsQuestActive(QuestTag))
		return 0;
	else return ActiveQuests[QuestTag].CurrentTarget.StageInt;
}

inline bool AThirdPersonPlayerController::IsQuestFocused(const FGameplayTag& QuestTag)
{
	return FocusedQuest == QuestTag;
}

bool AThirdPersonPlayerController::HasFocusedQuest()
{
	return FocusedQuest.IsValid();
}

void AThirdPersonPlayerController::AddActiveQuest_Implementation(const FGameplayTag& QuestTag)
{
	auto Quest = QuestSubsystem->GetQuest(QuestTag);
	FS_PlayerQuestHandler HoldQuest;
	HoldQuest.CurrentTarget.StageInt = Quest.Targets[1].StageInt;
	HoldQuest.CurrentTarget.IsCompleted = false;
	HoldQuest.CurrentTarget.TargetDescription = Quest.Targets[1].TargetDescription;
	HoldQuest.CurrentTarget.TargetName = Quest.Targets[1].TargetName;
	HoldQuest.IsFinished = false;
	ActiveQuests.Add(QuestTag,HoldQuest);
}

void AThirdPersonPlayerController::RemoveActiveQuest(const FGameplayTag& QuestTag)
{
	ActiveQuests.Remove(QuestTag);
}

void AThirdPersonPlayerController::GotoNextStage_Implementation(const FGameplayTag& QuestTag)
{
	if (!IsQuestActive(QuestTag))
	{
		GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
			   FString::Printf(TEXT("Class : %s,Function: %s,Quest is not active: QuestTag: %s"),
				  *GetName(),*FString(__FUNCTION__),
				  *QuestTag.ToString()));
		return;
	}
	auto Quest = QuestSubsystem->GetQuest(QuestTag);
	auto &HoldQuest = ActiveQuests[QuestTag];
	for (int i = 0; i < Quest.Targets.Num(); ++i)
	{
		if (Quest.Targets[i].StageInt == HoldQuest.CurrentTarget.StageInt)
		{
			//正常推进任务
			if (i+1 < Quest.Targets.Num())
			{
				HoldQuest.CurrentTarget.StageInt = Quest.Targets[i+1].StageInt;
				HoldQuest.CurrentTarget.TargetDescription = Quest.Targets[i+1].TargetDescription;
				HoldQuest.CurrentTarget.TargetName = Quest.Targets[i+1].TargetName;
				HoldQuest.CurrentTarget.IsCompleted = false;
			}
			//任务结束
			else
			{
				QuestFinished(QuestTag);
			}
			break;
		}
	}
}

bool AThirdPersonPlayerController::SetFocusedQuest_Implementation(const FGameplayTag& QuestTag)
{
	if (IsQuestActive(QuestTag))
	{
		FocusedQuest = QuestTag;
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
		   FString::Printf(TEXT("Class : %s,Function: %s,Quest is not active: QuestTag: %s"),
			  *GetName(),*FString(__FUNCTION__),
			  *QuestTag.ToString()));
		return false;
	}
}
























