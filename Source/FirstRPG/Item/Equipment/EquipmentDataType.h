#pragma once
#include "CoreMinimal.h"
#include "EquipmentDataType.generated.h"

class AMeleeWeapon;
class ARangeWeapon;
class AArmor;
class AShield;

USTRUCT(Blueprintable,BlueprintType)
struct FS_EquipmentRef
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	AMeleeWeapon * MeleeWeapon = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	ARangeWeapon * RangeWeapon = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	AShield * Shield = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	AArmor * Armor = nullptr;
};

USTRUCT(Blueprintable,BlueprintType)
struct FS_DefaultEquipmentStatus
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	FDataTableRowHandle MeleeWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	FDataTableRowHandle RangeWeapon;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	FDataTableRowHandle Shield;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	FDataTableRowHandle Armor;
};

UENUM(BlueprintType)
enum class E_WeaponKind : uint8
{
	None,
	MeleeWeapon,
	RangeWeapon
};