// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstRPG/Item//GeneralItemActor.h"
#include "FirstRPG/Item//Equipment/EquipmentInstance.h"
#include "EquippableItemActor.generated.h"

UCLASS(Blueprintable)
class FIRSTRPG_API AEquippableItemActor : public AGeneralItemActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
};
