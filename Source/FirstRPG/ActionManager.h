// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <map>
#include <vector>
#include <string>
#include "S_ActionInterruptTableRow.h"
#include "ActionManager.generated.h"
#define MAX_ACTION_NUM  100

USTRUCT(BlueprintType)
struct FActionGraphRow
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<uint8> Row;
};

UCLASS( ClassGroup=(Action), meta=(BlueprintSpawnableComponent),Blueprintable )
class FIRSTRPG_API UActionManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionManager();

	UPROPERTY(VisibleAnywhere, Category = "ActionManager")
	TMap<FString, int32> ActionName2Num;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionManager")
	UDataTable* ActionTable;
  
    UPROPERTY(VisibleAnywhere, Category = "ActionManager")
    TArray<FActionGraphRow> ActionGraph;

	UPROPERTY(VisibleAnywhere, Category = "ActionManager")
	TArray<FString> ActionNum2Name;

	UFUNCTION(BlueprintCallable)
	bool CanExe(const FString& ActionName);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void EndAction(const FString& ActionName) {
		int Num = GetActionNum(ActionName);
		if (!ActiveActions.Find(Num)) {
			UE_LOG(LogTemp, Error, TEXT("Action %s not active"), *ActionName);
		}
		ActiveActions.Remove(Num);
	}


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	TSet<int> ActiveActions;
    FORCEINLINE int GetActionNum(const FString& ActionName) {
		if (!ActionName2Num.Find(ActionName)) {
			UE_LOG(LogTemp, Error, TEXT("Haven't Action Name: %s"), *ActionName);
		}
        return ActionName2Num[ActionName];
    }
	void ReadActionTable();
	void PrintActiveAction();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

#define OUT_A_CANBEINTBY_B(A,B) (ActionGraph[GetActionNum(B)].Row[GetActionNum(A)])