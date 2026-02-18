// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralItemActor.h"
#include "FirstRPG/Debug/Debug.h"

// Sets default values
AGeneralItemActor::AGeneralItemActor()
{
	SetTickableWhenPaused(true);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGeneralItemActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (ItemInstance) return;
	//读取数据表格信息并设置相关元素
	if (ItemInfoHandle.IsNull())
	{
		CPP_LOG(Error, FString(TEXT("ItemInfoHandle is null")));
		return;
	}
	auto ItemInfo = ItemInfoHandle.GetRow<FS_GeneralItemInfo>(
		FString::Printf(TEXT("GetRow failed, DB:%s Row:%s"),*ItemInfoHandle.DataTable.GetPathName(),*ItemInfoHandle.RowName.ToString()));
	ItemInstance = GeneralItemInstance();
	ItemInstance->Initialize(ItemInfo);
	
}

// Called every frame
void AGeneralItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AGeneralItemActor::GetItemInfo() const
{
	return ItemInstance->GetItemInfo();
}

UGeneralItemInstance* AGeneralItemActor::GeneralItemInstance()
{
	return NewObject<UGeneralItemInstance>(this);
}

