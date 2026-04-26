// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquippableItemActor.h"
#include "Weapon.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTRPG_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
};
