// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "S_ActionInterruptTableRow.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FIRSTRPG_API FS_ActionInterruptTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionInterruptTableRow")
	FString ActionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionInterruptTableRow")
	TArray<uint8> InterrputionVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActionInterruptTableRow")
	int32 ActionNum;
};
