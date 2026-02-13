// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NativeGameplayTags.h"
#include "ActionManager.generated.h"
#define MAX_ACTION_NUM  100

USTRUCT()
struct FS_ActionInterruptTableRow : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ActionTag;
	//这个动作可以被哪些其他动作打断或共存
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<FGameplayTag> CanBeInterrupt;
};

UCLASS( ClassGroup=(Action), meta=(BlueprintSpawnableComponent),Blueprintable )
class FIRSTRPG_API UActionManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionManager();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool CanExe(const FGameplayTag& ActionName);
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void EndAction(const FGameplayTag& Action);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionManager")
	UDataTable* ActionTable;
	TMap<FGameplayTag,TSet<FGameplayTag>> ActionTableMap;
	TSet<FGameplayTag> ActiveActions;

	void ReadActionTable();
};
