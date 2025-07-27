// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <map>
#include <vector>
#include <string>
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

	UPROPERTY(VisibleAnywhere)
	TMap<FString, int32> ActionName2Num;

    // 将 ActionGraph 的类型从 TArray<TArray<bool>> 改为 TArray<TArray<uint8>>
    // Unreal Engine 不支持 TArray<bool> 作为 TArray 的值类型
    UPROPERTY(VisibleAnywhere)
    TArray<FActionGraphRow> ActionGraph;
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
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
