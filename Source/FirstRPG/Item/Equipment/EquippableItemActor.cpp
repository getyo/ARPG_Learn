// Fill out your copyright notice in the Description page of Project Settings.


#include "EquippableItemActor.h"
#include "FirstRPG/Debug/Debug.h"


void AEquippableItemActor::BeginPlay()
{
	Super::BeginPlay();
	if (!ItemInstance && IsA(AEquippableItemActor::StaticClass()))
	{
		ItemInstance = UEquipmentInstance::EquipmentInstanceFactory(ItemInfoHandle);
		if (!ItemInstance)
		{
			CPP_LOG(Warning,"Cannot get valid ItemInstance");
		}
	}
}