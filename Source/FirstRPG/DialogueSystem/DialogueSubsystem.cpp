// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSubsystem.h"
#include "DialogueComponent.h"
#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "../QuestSystem/QuestionSubsystem.h"
#include "../QuestSystem/DialogueCondition.h"
#include "FirstRPG/Character/CharacterManagerSubsystem.h"

void UDialogueSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	//获取资源加载模块
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	//通过类名找到所有对话资产
	TArray<FAssetData> AssetList;
	AssetRegistry.GetAssetsByClass(FTopLevelAssetPath(UDialogueDataAsset::StaticClass()->GetClassPathName()), AssetList, true);
	
	//构建映射
	for (auto Elem : AssetList)
	{
		if (auto Asset = Cast<UDialogueDataAsset>(Elem.GetAsset()))
		{
			//是否存在当前任务
			if (!DialogueAssetMap.Contains(Asset->RelativeQuestID))
			{
				DialogueAssetMap.Add(Asset->RelativeQuestID,FInQuestTargetDialogMap());
			}
			FInQuestTargetDialogMap * InQuestTargetDialogMapPtr = DialogueAssetMap.Find(Asset->RelativeQuestID);
			//是否存在当前阶段
			if (!InQuestTargetDialogMapPtr->Map.Contains(Asset->Stage))
			{
				InQuestTargetDialogMapPtr->Map.Add(Asset->Stage,Asset);
				
			}
		}
	}
}

void UDialogueSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UDialogueSubsystem::StartDialogue(const FString& QuestID, int Stage, AThirdPersonPlayerController* PlayerController)
{
	if (!DialogueAssetMap.Contains(QuestID))
	{
		GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
			FString::Printf(TEXT("Class : %s, Cannot Find Quest's Dialogue: QuestID : %s"),
				*GetClass()->GetName(),*QuestID));
		return false;
	}
	FInQuestTargetDialogMap * InQuestTargetDialogMapPtr = DialogueAssetMap.Find(QuestID);
	//是否存在当前阶段
	if (!InQuestTargetDialogMapPtr->Map.Contains(Stage))
	{
		GEngine->AddOnScreenDebugMessage(-1,20.f,FColor::Red,
	FString::Printf(TEXT("Class : %s, Cannot Find QuestStage's Dialogue: QuestID : %s , Stage : %d"),
		*GetClass()->GetName(),*QuestID,Stage));
		return false;
	}
	
	CurrentQuest = QuestID;
	CurrentStage = Stage;
	TextIt = 0;
	CurrentDialogueAsset = InQuestTargetDialogMapPtr->Map[Stage];
	CurrentPlayerController = PlayerController;
	
	auto GI = GetWorld()->GetGameInstance();
	auto CharacterManager = GI->GetSubsystem<UCharacterManagerSubsystem>();
	if (!CharacterManager) return false;
	TSet<FString> Characters;
	//把所有相关的Character的InDialogye设置为true
	for (auto Elem : CurrentDialogueAsset->DialogueLines)
	{
		Characters.Add(Elem.SpeakerName);
	}
	for (auto CharacterID : Characters)
	{
		if (auto Character = CharacterManager->GetCharacterByName(CharacterID))
		{
			UDialogueComponent* DialogueComp = Character->FindComponentByClass<UDialogueComponent>();
			if (DialogueComp)
			{
				DialogueComp->SetInDialogue(true);
			}
		}
			
	}
	return true;
}

bool UDialogueSubsystem::EndDialogue()
{
	auto GI = GetWorld()->GetGameInstance();
	auto CharacterManager = GI->GetSubsystem<UCharacterManagerSubsystem>();
	if (!CharacterManager) return false;
	TSet<FString> Characters;
	//把所有相关的Character的InDialogye设置为true
	for (auto Elem : DialogueAssetMap[CurrentQuest].Map[CurrentStage]->DialogueLines)
	{
		Characters.Add(Elem.SpeakerName);
	}
	for (auto CharacterName : Characters)
	{
		if (auto Character = CharacterManager->GetCharacterByName(CharacterName))
		{
			UDialogueComponent* DialogueComp = Character->FindComponentByClass<UDialogueComponent>();
			if (DialogueComp)
			{
				DialogueComp->SetInDialogue(false);
			}
		}
			
	}
	//广播对话完成
	auto QuestSys = GI->GetSubsystem<UQuestionSubsystem>();
	QuestSys->BroadcastFinish(FS_QuestTargetData(E_QuestTargetConditionType::Talk,
		TalkConditionTag,
		CurrentDialogueAsset->TargetTag,
		0,
		CurrentPlayerController,
		nullptr));
	
	//重置部分相关变量
	CurrentDialogueAsset = nullptr;
	CurrentPlayerController = nullptr;
	TextIt = 0;
	return true;
}



















