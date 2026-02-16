// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneralItemActor.h"
#include "StaticItem.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTRPG_API AStaticItem : public AGeneralItemActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="StaticMesh")
	UStaticMeshComponent* MeshComp;
};
