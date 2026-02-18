#include "EquipmentInstance.h"

#include "FirstRPG/Debug/Debug.h"

void UEquipmentInstance::Initialize(FS_GeneralItemInfo * ItemInfo)
{
	Super::Initialize(ItemInfo);
	FS_EquippableItemInfo * EquipmentInfo = static_cast<FS_EquippableItemInfo*>(ItemInfo);
	EquipmentCategory = EquipmentInfo->EquipmentCategory;
	ATKorDFS = EquipmentInfo->ATKorDFS;
	LeverRestriction = EquipmentInfo->LeverRestriction;
	SocketName = EquipmentInfo->SocketName;
}
