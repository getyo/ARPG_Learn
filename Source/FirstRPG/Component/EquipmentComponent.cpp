// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"

#include "Components/SceneCaptureComponent2D.h"
#include "FirstRPG/Item//Equipment/StMeleeWeapon.h"
#include "FirstRPG/Debug/Debug.h"
#include "FirstRPG/Item/Equipment/SkArmor.h"
#include "FirstRPG/Item/Equipment/StRangeWeapon.h"
#include "FirstRPG/Item/Equipment/StShield.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetTickableWhenPaused(true);
	// ...
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	OwnerMesh = Cast<UMeshComponent>(GetOwner()->GetComponentByClass(UMeshComponent::StaticClass()));
	if (!OwnerMesh)
	{
		CPP_LOG(Error,"Get CharacterMesh failed");
		return;
	}
	CaptureComponent = Cast<USceneCaptureComponent2D>(GetOwner()->GetComponentByClass(USceneCaptureComponent2D::StaticClass()));
	if (!CaptureComponent)
	{
		CPP_LOG(Error,"Get SceneCaptureCmp failed");
		return;
	}
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentComponent::AddEquippableItem(AEquippableItemActor* EquippableItem)
{
	auto EquipmentInst = Cast<UEquipmentInstance>(EquippableItem->GetItemInstance());
	if (!EquipmentInst)
	{
		CPP_LOG(Warning,FString::Printf(TEXT("Item is not equipment:%s"),*EquippableItem->GetItemInfo()));
	}
	auto EquipmentPtr = EquipmentName2Obj.Find(EquipmentInst->GetItemName());
	if (!EquipmentPtr)
	{
		EquipmentName2Obj.Add(EquipmentInst->GetItemName(), EquipmentInst);
		EquippableItems.Add(EquipmentInst);
	}
	else if (EquipmentInst->CanBeStacked())
	{
		(*EquipmentPtr)->AddCnt(EquipmentInst->GetTotalCnt());
	}
	else
	{
		EquippableItems.Add(EquipmentInst);
	}
}

bool UEquipmentComponent::SetEquippedMeleeWeapon(UEquipmentInstance * Instance)
{
	if (!EquippableItems.Contains(Instance))
	{
		CPP_LOG(Warning,FString::Printf(TEXT("Have not been added to EquippableItems:%s"),*Instance->GetItemInfo()));
		return false;
	}
	if (Instance->GetCategory() != E_EquipmentCategory::MeleeWeapon)
	{
		CPP_LOG(Warning,FString::Printf(TEXT("%s is not MeleeWeapon"),*Instance->GetItemName()));
		return false;
	}
	
	if (EquipmentStatus.MeleeWeapon)
	{
		RemoveMeleeWeapon();
	}
	if (Instance->GetStMesh())
	{
		//生成并附加到角色的网格体插槽上
		EquipmentStatus.MeleeWeapon = SpawnAndAttach<AStMeleeWeapon>(Instance,OwnerMesh);
		CaptureComponent->ShowOnlyActors.Add(EquipmentStatus.MeleeWeapon);
		return true;
	}
	return false;
}

void UEquipmentComponent::RemoveMeleeWeapon()
{
	CaptureComponent->ShowOnlyActors.Remove(EquipmentStatus.MeleeWeapon);
	EquipmentStatus.MeleeWeapon->Destroy(true);
	EquipmentStatus.MeleeWeapon = nullptr;
}

bool UEquipmentComponent::SetEquipedEquipment(UEquipmentInstance* Instance)
{
	switch (Instance->GetCategory())
	{
	case E_EquipmentCategory::MeleeWeapon:
		{
			return SetEquippedMeleeWeapon(Instance);
		}
	case E_EquipmentCategory::RangeWeapon:
		{
			return SetEquippedRangeWeapon(Instance);
		}
	case E_EquipmentCategory::Shield:
		{
			return SetEquippedShield(Instance);
		}
	case E_EquipmentCategory::Armor:
		{
			return SetEquippedArmor(Instance);
		}
	default:
		{
			CPP_LOG(Warning,"Unknown EquipmentCategory");
			return false;
		}
	}
}

bool UEquipmentComponent::SetEquippedRangeWeapon(UEquipmentInstance* Instance)
{
	if (!Instance || !EquippableItems.Contains(Instance))
	{
		CPP_LOG(Warning,FString::Printf(TEXT("Have not been added to EquippableItems:%s"),*Instance->GetItemInfo()));
		return false;
	}
	if (Instance->GetCategory() != E_EquipmentCategory::RangeWeapon)
	{
		CPP_LOG(Warning,FString::Printf(TEXT("%s is not RangeWeapon"),*Instance->GetItemName()));
		return false;
	}
	
	if (EquipmentStatus.RangeWeapon)
	{
		RemoveRangeWeapon();
	}
	if (Instance->GetStMesh())
	{
		EquipmentStatus.RangeWeapon = SpawnAndAttach<AStRangeWeapon>(Instance, OwnerMesh);
       
		if (EquipmentStatus.RangeWeapon)
		{
			CaptureComponent->ShowOnlyActors.Add(EquipmentStatus.RangeWeapon);
			return true;
		}
	}
	return false;
}

void UEquipmentComponent::RemoveRangeWeapon()
{
	CaptureComponent->ShowOnlyActors.Remove(EquipmentStatus.RangeWeapon);
	EquipmentStatus.RangeWeapon->Destroy(true);
	EquipmentStatus.RangeWeapon = nullptr;
}

bool UEquipmentComponent::SetEquippedShield(UEquipmentInstance* Instance)
{
	if (!Instance || !EquippableItems.Contains(Instance))
	{
		CPP_LOG(Warning,FString::Printf(TEXT("Have not been added to EquippableItems:%s"),*Instance->GetItemInfo()));
		return false;
	}
	if (Instance->GetCategory() != E_EquipmentCategory::Shield)
	{
		CPP_LOG(Warning,FString::Printf(TEXT("%s is not Shield"),*Instance->GetItemName()));
		return false;
	}

	// 1. 销毁旧盾牌
	if (EquipmentStatus.Shield)
	{
		RemoveShield();
	}

	// 2. 生成新盾牌
	if (Instance->GetStMesh())
	{
		EquipmentStatus.Shield = SpawnAndAttach<AStShield>(Instance, OwnerMesh);
       
		if (EquipmentStatus.Shield)
		{
			CaptureComponent->ShowOnlyActors.Add(EquipmentStatus.Shield);
			return true;
		}
	}
	return false;
}

void UEquipmentComponent::RemoveShield()
{
	CaptureComponent->ShowOnlyActors.Remove(EquipmentStatus.Shield);
	EquipmentStatus.Shield->Destroy(true);
	EquipmentStatus.Shield = nullptr;
}

bool UEquipmentComponent::SetEquippedArmor(UEquipmentInstance* Instance)
{
	if (!Instance || !EquippableItems.Contains(Instance))
	{
		CPP_LOG(Warning,FString::Printf(TEXT("Have not been added to EquippableItems:%s"),*Instance->GetItemInfo()));
		return false;
	}
	if (Instance->GetCategory() != E_EquipmentCategory::Armor)
	{
		CPP_LOG(Warning,FString::Printf(TEXT("%s is not Armor"),*Instance->GetItemName()));
		return false;
	}

	
	if (EquipmentStatus.Armor)
	{
		RemoveShield();
	}
	//对于盔甲一般是骨骼网格体
	if (Instance->GetSkMesh())
	{
		if (!Instance || !OwnerMesh) return false;
		auto Armor = OwnerMesh->GetWorld()->SpawnActorDeferred<ASkArmor>(ASkArmor::StaticClass(),OwnerMesh->GetComponentTransform());
		if (Armor)
		{
			//完成Armor的生成
			Armor->SetItemInstance(Instance);
			Armor->FinishSpawning(OwnerMesh->GetComponentTransform());
			
			//添加到角色上
			auto SkOwnerMesh = Cast<USkinnedMeshComponent>(OwnerMesh);
			Armor->GetSkeletalMeshComponent()->SetLeaderPoseComponent(SkOwnerMesh);
			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
			Armor->AttachToComponent(OwnerMesh,AttachmentTransformRules);
			//隐藏角色身体并设置保持动画更新
			SkOwnerMesh->SetHiddenInGame(true);
			SkOwnerMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
			
			CaptureComponent->ShowOnlyActors.Add(Armor);
			return true;
		}
		return false;
	}
	return false;
}

TArray<UEquipmentInstance*> UEquipmentComponent::GetEquippableItems()
{
	return EquippableItems;
}
