// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstRPG/Item/GeneralItemInstance.h"
#include "EquipmentInstance.generated.h"

UENUM(BlueprintType)
enum class E_EquipmentCategory : uint8
{
	None = 0,
	MeleeWeapon,
	RangeWeapon,
	Armor,
	Shield,
};

USTRUCT(BlueprintType)
struct FS_EquippableItemInfo :public FS_GeneralItemInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "EquipmentParas")
	E_EquipmentCategory EquipmentCategory = E_EquipmentCategory::None;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "EquipmentParas")
	float ATKorDFS = 0;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "EquipmentParas")
	int LeverRestriction = 0;
	//只有当Item为静态网格时这个变量才生效，用于指定装备在角色身上的位置
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "EquipmentParas")
	FString EquippedSocketName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "EquipmentParas")
	FString ArmedSocketName;
};


UCLASS(Blueprintable,BlueprintType)
class FIRSTRPG_API UEquipmentInstance : public UGeneralItemInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "EquipmentParas")
	E_EquipmentCategory EquipmentCategory = E_EquipmentCategory::None;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "EquipmentParas")
	float ATKorDFS = -1;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "EquipmentParas")
	int LeverRestriction = -1;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "EquipmentParas")
	FString EquippedSocketName;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "EquipmentParas")
	FString ArmedSocketName;
public:
	virtual void Initialize(FS_GeneralItemInfo* ItemInfo) override ;
	E_EquipmentCategory GetCategory() const {return EquipmentCategory;}
	float GetATKorDFS() const { return ATKorDFS; }
	int32 GetLevelRestriction() const { return LeverRestriction; }
	FString GetEquippedSocketName() const { return EquippedSocketName; }
	FString GetArmedSocketName() const { return ArmedSocketName; }
	UFUNCTION(BlueprintCallable,Category = "Equipment | Instance")
	static UEquipmentInstance * EquipmentInstanceFactory(const FDataTableRowHandle &DataSource);
};
