// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSubsystem.h"
#include "DialogueComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "FirstRPG/CharacterManagerSubsystem.h"

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
		if (auto * Asset = Cast<UDialogueDataAsset>(Elem.GetAsset()))
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
				InQuestTargetDialogMapPtr->Map.Add(Asset->Stage,Asset->DialogueLines);
			}
		}
	}
}

void UDialogueSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UDialogueSubsystem::StartDialogue(const FString& QuestID, int Stage)
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
	
	auto GI = GetWorld()->GetGameInstance();
	auto CharacterManager = Cast<UCharacterManagerSubsystem>(GI);
	if (!CharacterManager) return false;
	TSet<FString> Characters;
	//把所有相关的Character的InDialogye设置为true
	for (auto Elem : InQuestTargetDialogMapPtr->Map[Stage])
	{
		Characters.Add(Elem.SpeakerID);
	}
	for (auto CharacterID : Characters)
	{
		if (auto Character = CharacterManager->GetCharacterByID(CharacterID))
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
	auto CharacterManager = Cast<UCharacterManagerSubsystem>(GI);
	if (!CharacterManager) return false;
	TSet<FString> Characters;
	//把所有相关的Character的InDialogye设置为true
	for (auto Elem : DialogueAssetMap[CurrentQuest].Map[CurrentStage])
	{
		Characters.Add(Elem.SpeakerID);
	}
	for (auto CharacterID : Characters)
	{
		if (auto Character = CharacterManager->GetCharacterByID(CharacterID))
		{
			UDialogueComponent* DialogueComp = Character->FindComponentByClass<UDialogueComponent>();
			if (DialogueComp)
			{
				DialogueComp->SetInDialogue(false);
			}
		}
			
	}
	return true;
}



















