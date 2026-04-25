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
	
	if (!ItemInstance && GetClass() == AGeneralItemActor::StaticClass())
	{
		ItemInstance = UGeneralItemInstance::GeneralItemInstanceFactory(ItemInfoHandle);
		if (!ItemInstance)
		{
			CPP_LOG(Warning,"Cannot get valid ItemInstance");
		}
	}
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

