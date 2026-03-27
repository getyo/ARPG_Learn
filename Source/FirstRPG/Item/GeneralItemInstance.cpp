// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralItemInstance.h"

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


