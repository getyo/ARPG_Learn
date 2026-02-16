// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Armor.h"
#include "EquipmentInstance.h"
#include "SkArmor.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTRPG_API ASkArmor : public AArmor
{
	GENERATED_BODY()
	ASkArmor();
protected:
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="StaticMesh")
	USkeletalMeshComponent* MeshComp;
public:
	USkeletalMeshComponent* GetSkeletalMeshComponent() const{return MeshComp;}
};
