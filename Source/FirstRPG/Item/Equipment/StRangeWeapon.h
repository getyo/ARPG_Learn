// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RangeWeapon.h"
#include "EquipmentInstance.h"
#include "StRangeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTRPG_API AStRangeWeapon : public ARangeWeapon
{
	GENERATED_BODY()
	AStRangeWeapon();
protected:
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="StaticMesh")
	UStaticMeshComponent* MeshComp;
public:
	UStaticMeshComponent* GetStaticMeshComponent() const{return MeshComp;}
};
