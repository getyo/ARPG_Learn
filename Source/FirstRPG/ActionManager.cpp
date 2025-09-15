// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionManager.h"

// Sets default values for this component's properties
UActionManager::UActionManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	FString FileContent;
	FString FilePath = FPaths::ProjectContentDir() + TEXT("ActionMap.txt");
	FFileHelper::LoadFileToString(FileContent, *FilePath);
	if (FileContent.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("ActionMap.txt is empty or not found!"));
		return;
	}
	TArray<FString> Lines;
	FileContent.ParseIntoArrayLines(Lines);

	FString ActionName;
	FString ActionNum;
	int ActionCnt, i = 0;
	ActionCnt = FCString::Atoi(*Lines[0]);
	++i;
	while (i <= ActionCnt) {
		TArray<FString> Words;
		Lines[i].ParseIntoArray(Words, TEXT(" "), true);
		ActionName = TCHAR_TO_UTF8(*Words[0]);
		ActionNum = TCHAR_TO_UTF8(*Words[1]);
		ActionName2Num.Add(ActionName, FCString::Atoi(*ActionNum));
		++i;
	}
	ActionGraph.SetNum(ActionCnt);
	for (auto& line : ActionGraph) {
		line.Row.SetNum(ActionCnt);
	}
	bool b;
	//UE_LOG(LogTemp, Error, TEXT("Current Line: %d,Total Line: %d, ActionCnt: %d"), i, Lines.Num(), ActionCnt);
	for (int j = 0; j < ActionCnt; ++j) {
		TArray<FString> Bits;
		Lines[i + j].ParseIntoArray(Bits, TEXT(" "), true);
		//UE_LOG(LogTemp, Error, TEXT("ActionGraphOut: \n"));
		for (int col = 0; col < ActionCnt; ++col) {
			b = FCString::Atoi(*Bits[col]) != 0;
			ActionGraph[j].Row[col] = b;
		}
	}
	// ...
}


// Called when the game starts
void UActionManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UActionManager::CanExe(const FString &ActionName) {
	int RequsetAcNum = GetActionNum(ActionName);
	if (!ActiveActions.Num()) {
		ActiveActions.Add(RequsetAcNum);
		return true;
	}
	bool CanExe = true;
	for (auto ActiveAcNum : ActiveActions) {
		CanExe = CanExe && ActionGraph[RequsetAcNum].Row[ActiveAcNum];
	}
	if (CanExe) {
		ActiveActions.Add(RequsetAcNum);
		return true;
	}
	else return false;
		
}


// Called every frame
void UActionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

