// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralItemInstance.h"
#include "FirstRPG/Debug/Debug.h"
UGeneralItemInstance::UGeneralItemInstance()
{
}

void UGeneralItemInstance::Initialize(FS_GeneralItemInfo* ItemInfo)
{
	DisplayName = ItemInfo->DisplayName;
	OwnerActor = ItemInfo->OwnerActor;
	Icon = ItemInfo->Icon;
	StMesh = ItemInfo->StMesh;
	SkMesh = ItemInfo->SkMesh;
	TotalCnt += ItemInfo->PickUpCnt;
	_CanBeStacked = ItemInfo->CanBeStacked;
	_BPClassToSpawn = ItemInfo->BPClassToSpawn;
}

void UGeneralItemInstance::AddCnt(int Cnt)
{
	TotalCnt += Cnt;
}

UGeneralItemInstance* UGeneralItemInstance::GeneralItemInstanceFactory(const FDataTableRowHandle& DataSource)
{
	if (DataSource.IsNull())
	{
		CPP_STATIC_LOG(UGeneralItemInstance::StaticClass()->GetName(),Error, FString(TEXT("ItemInfoHandle is null")));
		return nullptr;
	}
	auto ItemInfo = DataSource.GetRow<FS_GeneralItemInfo>(
		FString::Printf(TEXT("GetRow failed, DB:%s Row:%s"),*DataSource.DataTable.GetPathName(),*DataSource.RowName.ToString()));
	ASSERT_STATIC(ItemInfo->StaticStruct() == FS_GeneralItemInfo::StaticStruct(),UGeneralItemInstance::StaticClass()->GetName());
	auto ItemInstance = NewObject<UGeneralItemInstance>();
	ItemInstance->DataTableRow = DataSource;
	ItemInstance->Initialize(ItemInfo);
	return ItemInstance;
}


