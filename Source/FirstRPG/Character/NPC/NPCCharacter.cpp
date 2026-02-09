// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"

#include "FirstRPG/Character/Player/ThirdPersonPlayerController.h"
#include "Kismet/GameplayStatics.h"

bool ANPCCharacter::IsQuestRelative_Implementation()
{
	return IsQuestActor;
}

TArray<FGameplayTag> ANPCCharacter::GetRelativeQuest_Implementation()
{
	TArray<FGameplayTag> tags;
	RelativeQuestMap.GetKeys(tags);
	return tags;
}

void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ANPCCharacter::SendTargetFinish(const FGameplayTag& ActionTag, int Cnt, AController* TargetInstigator)
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
						CharacterTag,
						Cnt,
						TargetInstigator,
						nullptr
						));
				}
			}
		}
	}
}
