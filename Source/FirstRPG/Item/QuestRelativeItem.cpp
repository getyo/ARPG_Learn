// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestRelativeItem.h"

#include "FirstRPG/Character/NPC/NPCCharacter.h"
#include "Kismet/GameplayStatics.h"


bool AQuestRelativeItem::IsQuestRelative_Implementation()
{
	return IsQuestActor;
}

TArray<FGameplayTag> AQuestRelativeItem::GetRelativeQuest_Implementation()
{
	TArray<FGameplayTag> tags;
	RelativeQuestMap.GetKeys(tags);
	return tags;
}

void AQuestRelativeItem::BeginPlay()
{
	Super::BeginPlay();
}

void AQuestRelativeItem::SendTargetFinish(const FGameplayTag& ActionTag, int Cnt, AController* TargetInstigator)
{
	if (!PlayerController)
		PlayerController = Cast<AThirdPersonPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!QuestSubsystem)
	{
		UGameInstance* GI = GetWorld()->GetGameInstance();
		QuestSubsystem= GI->GetSubsystem<UQuestionSubsystem>();
	}
	for (auto &Pair : RelativeQuestMap)
	{
		for (auto &Target : Pair.Value.Targets)
		{
			if (Target.StageInt == PlayerController->GetQuestStageInt(Pair.Key))
			{
				if (ActionTag == Target.ActionTag){
					QuestSubsystem->BroadcastFinish(FS_QuestTargetData(
						Pair.Key,
						Target.StageInt,
						Target.ActionTag,
						GetItemInstance()->GetItemTag(),
						Cnt,
						TargetInstigator,
						nullptr
						));
				}
			}
		}
	}
}