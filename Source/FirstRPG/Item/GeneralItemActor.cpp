// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralItemActor.h"
#include "FirstRPG/Debug/Debug.h"
#include "FirstRPG/SaveSystem/SaveLoadSystem.h"

// Sets default values
AGeneralItemActor::AGeneralItemActor()
{
	SetTickableWhenPaused(true);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGeneralItemActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	FGuid Guid = FGuid::NewGuid();
	ItemID = Guid.ToString();
}

void AGeneralItemActor::PreDuplicate(FObjectDuplicationParameters& DupParams)
{
	Super::PreDuplicate(DupParams);
	FGuid Guid = FGuid::NewGuid();
	ItemID = Guid.ToString();
}

// Called when the game starts or when spawned
void AGeneralItemActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!ItemInstance && GetClass() == AGeneralItemActor::StaticClass())
	{
		ItemInstance = UGeneralItemInstance::GeneralItemInstanceFactory(ItemInfoHandle);
		ItemInfoHandle = ItemInstance->GetDataTableRow();
		if (!ItemInstance)
		{
			CPP_LOG(Warning,"Cannot get valid ItemInstance");
		}
		if (!ItemInstance->Holder) RegisterItem();
	}
}

// Called every frame
void AGeneralItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGeneralItemActor::RegisterItem() 
{
	UGameInstance* GI = GetGameInstance();
	if (GI)
	{
		//存档子系统注册
		if (USaveLoadSystem* SaveLoadSystem = GI->GetSubsystem<USaveLoadSystem>())
		{
			SaveLoadSystem->Register(this);
		}
	}
}

void AGeneralItemActor::UnRegisterItem()
{
	UGameInstance* GI = GetGameInstance();
	if (GI)
	{
		//存档子系统注册
		if (USaveLoadSystem* SaveLoadSystem = GI->GetSubsystem<USaveLoadSystem>())
		{
			SaveLoadSystem->UnRegister(this);
		}
	}
}

void AGeneralItemActor::SaveItemData_Implementation(FItemSaveData& Data)
{
	Data.IsStatic = IsStatic;
	Data.ItemClass = this->GetClass();
	Data.ItemInfoRowHandler = this->ItemInfoHandle;
	auto Transform = this->GetActorTransform();
	Data.Location = Transform.GetLocation();
	Data.Rotation = Transform.GetRotation().Rotator();
	Data.Scale = Transform.GetScale3D();
	Data.Name = GetItemInstance()->GetItemName();
}

void AGeneralItemActor::LoadItemData_Implementation(const FItemSaveData& Data)
{
	IsStatic = Data.IsStatic;
	FTransform Transform;
	Transform.SetLocation(Data.Location);
	Transform.SetRotation(Data.Rotation.Quaternion());
	Transform.SetScale3D(Data.Scale);
}

AGeneralItemActor* AGeneralItemActor::SpawnItem(UObject* WorldContextObject, ::TSubclassOf<AGeneralItemActor> ItemClass,
                                                UGeneralItemInstance* ItemInst,
                                                FTransform SpawnTransform)
{
	if (!WorldContextObject || !ItemClass) return nullptr;

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return nullptr;
	
	auto ItemActor = World->SpawnActorDeferred<AGeneralItemActor>(ItemClass, SpawnTransform);
	if (ItemActor)
	{
		
		ItemActor->SetItemInstance(ItemInst);
		ItemActor->IsStatic = false;
		ItemActor->FinishSpawning(SpawnTransform);
	}

	return ItemActor;
}

void AGeneralItemActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnRegisterItem();
}

FString AGeneralItemActor::GetItemInfo() const
{
	return ItemInstance->GetItemInfo();
}

