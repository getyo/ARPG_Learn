#include "EquipmentInstance.h"

#include "FirstRPG/Debug/Debug.h"

void UEquipmentInstance::Initialize(FS_GeneralItemInfo * ItemInfo)
{
	Super::Initialize(ItemInfo);
	FS_EquippableItemInfo * EquipmentInfo = static_cast<FS_EquippableItemInfo*>(ItemInfo);
	EquipmentCategory = EquipmentInfo->EquipmentCategory;
	ATKorDFS = EquipmentInfo->ATKorDFS;
	LeverRestriction = EquipmentInfo->LeverRestriction;
	EquippedSocketName = EquipmentInfo->EquippedSocketName;
	ArmedSocketName = EquipmentInfo->ArmedSocketName;
}

UEquipmentInstance* UEquipmentInstance::EquipmentInstanceFactory(const FDataTableRowHandle& DataSource)
{
	if (DataSource.IsNull())
	{
		CPP_STATIC_LOG(UEquipmentInstance::StaticClass()->GetName(),Error, FString(TEXT("ItemInfoHandle is null")));
		return nullptr;
	}
	auto ItemInfo = DataSource.GetRow<FS_EquippableItemInfo>(
		FString::Printf(TEXT("GetRow failed, DB:%s Row:%s"),*DataSource.DataTable.GetPathName(),*DataSource.RowName.ToString()));
	ASSERT_STATIC(ItemInfo->StaticStruct() == FS_EquippableItemInfo::StaticStruct(),UEquipmentInstance::StaticClass()->GetName());
	auto EquipmentInstance = NewObject<UEquipmentInstance>();
	EquipmentInstance->Initialize(ItemInfo);
	return EquipmentInstance;
}

