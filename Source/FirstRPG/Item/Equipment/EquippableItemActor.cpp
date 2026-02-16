// Fill out your copyright notice in the Description page of Project Settings.


#include "EquippableItemActor.h"
#include "FirstRPG/Debug/Debug.h"


void AEquippableItemActor::BeginPlay()
{
	Super::BeginPlay();
}

UGeneralItemInstance* AEquippableItemActor::GeneralItemInstance()
{
	return NewObject<UEquipmentInstance>(this);
}
