// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionManager.h"

// Sets default values for this component's properties
UActionManager::UActionManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
	// ...
}


// Called when the game starts
void UActionManager::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle Handle; // 注意：这个Handle可以不用存储，因为是单次执行
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UActionManager::ReadActionTable);
}

void UActionManager::ReadActionTable() {
	ActionGraph.Reset();
	FS_ActionInterruptTableRow* Row;
	//UE_LOG(LogTemp, Warning, TEXT("ActionTable ptr: %p"), ActionTable);
	if (ActionTable) {
		TArray<FName> RowNames = ActionTable->GetRowNames();
		int Num = 0, ActionCnt = ActionTable->GetRowMap().Num();

		for (auto Name : RowNames) {
			Row = ActionTable->FindRow<FS_ActionInterruptTableRow>(
				Name,
				FString::Printf(TEXT("Cann't Find Action :%s"), *Name.ToString()),
				true
			);
			if (Row) {
				ActionName2Num.Add(Name.ToString(), Row->ActionNum);
				ActionNum2Name.Add(Name.ToString());
				FActionGraphRow GraphRow; // 每次循环都新建一个
				GraphRow.Row.SetNum(ActionCnt);
				UE_LOG(LogTemp, Warning, TEXT("Load %s %d Actions\n\n"), *Name.ToString(), Row->ActionNum);
				for (int i = 0; i < ActionCnt; i++) {
					GraphRow.Row[i] = Row->InterrputionVector[i];
				}
				Num++;
				ActionGraph.Add(GraphRow);
				UE_LOG(LogTemp, Error, TEXT("ActionGraph rows :%d \n Row members: %d"), ActionGraph.Num(), ActionGraph[0].Row.Num());
			}
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No Action Table!"));
	}
	// ...
	UE_LOG(LogTemp, Error, TEXT("ActionGraph rows :%d \n Row members: %d"),ActionGraph.Num(),ActionGraph[0].Row.Num());
	for (int i = 0; i < ActionGraph.Num();++i) {
		for (int j = 0; j < ActionGraph[i].Row.Num();++j) {
			UE_LOG(LogTemp, Error, TEXT("%s\t %s \t %d \t"),
				*ActionNum2Name[i],*ActionNum2Name[j], ActionGraph[i].Row[j]);
		}
		UE_LOG(LogTemp, Error, TEXT("\n"));
	}
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

