// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeWeapon.h"
#include "EquipmentInstance.h"
#include "StMeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTRPG_API AStMeleeWeapon : public AMeleeWeapon
{
	GENERATED_BODY()
	AStMeleeWeapon();
protected:
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Melee Weapon | StaticMesh")
	UStaticMeshComponent* MeshComp;
public:
	UStaticMeshComponent* GetStaticMeshComponent() const{return MeshComp;}
};
