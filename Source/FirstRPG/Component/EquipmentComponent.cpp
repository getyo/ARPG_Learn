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
	if (GetOwner()->ActorHasTag("Player"))
	{
		CaptureComponent = Cast<USceneCaptureComponent2D>(GetOwner()->GetComponentByClass(USceneCaptureComponent2D::StaticClass()));
		if (!CaptureComponent)
		{
			CPP_LOG(Error,"Get SceneCaptureCmp failed");
			return;
		}
	}
	
	if (!DefaultEquipment.MeleeWeapon.IsNull())
	{
		auto Instance = UEquipmentInstance::EquipmentInstanceFactory(DefaultEquipment.MeleeWeapon);
		AddEquippableItem(Instance);
		SetEquippedMeleeWeapon(Instance);
	}
	if (!DefaultEquipment.RangeWeapon.IsNull())
	{
		auto Instance = UEquipmentInstance::EquipmentInstanceFactory(DefaultEquipment.RangeWeapon);
		AddEquippableItem(Instance);
		SetEquippedRangeWeapon(Instance);
	}
	if (!DefaultEquipment.Shield.IsNull())
	{
		auto Instance = UEquipmentInstance::EquipmentInstanceFactory(DefaultEquipment.Shield);
		AddEquippableItem(Instance);
		SetEquippedShield(Instance);
	}
	if (!DefaultEquipment.Armor.IsNull())
	{
		auto Instance = UEquipmentInstance::EquipmentInstanceFactory(DefaultEquipment.Armor);
		AddEquippableItem(Instance);
		SetEquippedArmor(Instance);
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
	AddEquippableItem(EquipmentInst);
}

void UEquipmentComponent::AddEquippableItem(UEquipmentInstance* EquipmentInst)
{
	auto EquipmentPtr = EquipmentName2Obj.Find(EquipmentInst->GetItemName());
	if (!EquipmentPtr)
	{
		EquipmentName2Obj.Add(EquipmentInst->GetItemName(), EquipmentInst);
		EquippableItems.Add(EquipmentInst);
		EquipmentInst->Holder = GetOwner();
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
	
	if (CurEquipmentRef.MeleeWeapon)
	{
		RemoveMeleeWeapon();
	}
	if (Instance->GetStMesh())
	{
		//生成并附加到角色的网格体插槽上
		CurEquipmentRef.MeleeWeapon = SpawnAtEquippedSocket<AStMeleeWeapon>(Instance,OwnerMesh);
		if (!CurEquipmentRef.MeleeWeapon)
		{
			CPP_LOG(Error,FString::Printf(TEXT("MeleeWeapon spawn failed,Item Name : %s"),*Instance->GetItemName()));
			return false;
		}
			
		return true;
	}
	return false;
}

void UEquipmentComponent::RemoveMeleeWeapon()
{
	if (GetOwner()->ActorHasTag("Player"))
		CaptureComponent->ShowOnlyActors.Remove(CurEquipmentRef.MeleeWeapon);
	if (UsingWeapon == CurEquipmentRef.MeleeWeapon)
		UsingWeapon = nullptr;
	CurEquipmentRef.MeleeWeapon->Destroy(true);
	CurEquipmentRef.MeleeWeapon = nullptr;
}

bool UEquipmentComponent::SetEquippedEquipment(UEquipmentInstance* Instance)
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
	
	if (CurEquipmentRef.RangeWeapon)
	{
		RemoveRangeWeapon();
	}
	if (Instance->GetStMesh())
	{
		CurEquipmentRef.RangeWeapon = SpawnAtEquippedSocket<AStRangeWeapon>(Instance, OwnerMesh);
       
		if (CurEquipmentRef.RangeWeapon)
		{	
			return true;
		}
	}
	return false;
}

void UEquipmentComponent::RemoveRangeWeapon()
{
	if (GetOwner()->ActorHasTag("Player"))
		CaptureComponent->ShowOnlyActors.Remove(CurEquipmentRef.RangeWeapon);
	if (UsingWeapon == CurEquipmentRef.RangeWeapon) 
		UsingWeapon = nullptr;
	CurEquipmentRef.RangeWeapon->Destroy(true);
	CurEquipmentRef.RangeWeapon = nullptr;
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
	if (CurEquipmentRef.Shield)
	{
		RemoveShield();
	}

	// 2. 生成新盾牌
	if (Instance->GetStMesh())
	{
		CurEquipmentRef.Shield = SpawnAtEquippedSocket<AStShield>(Instance, OwnerMesh);
       
		if (CurEquipmentRef.Shield)
		{
			return true;
		}
	}
	return false;
}

void UEquipmentComponent::RemoveShield()
{
	if (GetOwner()->ActorHasTag("Player"))
		CaptureComponent->ShowOnlyActors.Remove(CurEquipmentRef.Shield);
	if (CurEquipmentRef.Shield)
	{
		CurEquipmentRef.Shield->Destroy(true);
		CurEquipmentRef.Shield = nullptr;
	}
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

	
	if (CurEquipmentRef.Armor)
	{
		RemoveArmor();
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
			
			if (GetOwner()->ActorHasTag("Player"))
				CaptureComponent->ShowOnlyActors.Add(Armor);
			return true;
		}
		return false;
	}
	return false;
}

void UEquipmentComponent::RemoveArmor()
{
	if (GetOwner()->ActorHasTag("Player"))
		CaptureComponent->ShowOnlyActors.Remove(CurEquipmentRef.Armor);
	CurEquipmentRef.Armor->Destroy(true);
	CurEquipmentRef.Armor = nullptr;
}

TArray<UEquipmentInstance*> UEquipmentComponent::GetEquippableItems()
{
	return EquippableItems;
}

bool UEquipmentComponent::DrawWeapon(E_WeaponKind WeaponKind)
{
	switch (WeaponKind)
	{
	case E_WeaponKind::MeleeWeapon:
		{
			if (!CurEquipmentRef.MeleeWeapon) return false;
			auto MeleeInst = Cast<UEquipmentInstance>(CurEquipmentRef.MeleeWeapon->GetItemInstance());
			return ChangeUsingWeapon(MeleeInst);
		}
	case E_WeaponKind::RangeWeapon:
		{
			if (!CurEquipmentRef.RangeWeapon) return false;
			auto Inst = Cast<UEquipmentInstance>(CurEquipmentRef.RangeWeapon->GetItemInstance());
			return ChangeUsingWeapon(Inst);
		}
	default:break;
	}
	return false;
}

bool UEquipmentComponent::DrawShield()
{
	if (!CurEquipmentRef.Shield) return false;
	auto ShieldInst = Cast<UEquipmentInstance>(CurEquipmentRef.Shield->GetItemInstance());
	RemoveShield();
	CurEquipmentRef.Shield = SpawnAtArmedSocket<AStShield>(ShieldInst, OwnerMesh);
	return true;
}

void UEquipmentComponent::SheatheWeapon()
{
	if (UsingWeapon) DestroyUsingWeapon();
}

void UEquipmentComponent::SheatheShield()
{
	if (!CurEquipmentRef.Shield) return;
	auto ShieldInst = Cast<UEquipmentInstance>(CurEquipmentRef.Shield->GetItemInstance());
	RemoveShield();
	//在装备位置重新生成近战武器
	CurEquipmentRef.Shield = SpawnAtEquippedSocket<AStShield>(ShieldInst,OwnerMesh);
}

bool UEquipmentComponent::ChangeUsingWeapon(UEquipmentInstance* Instance)
{
	//如果当前武装着武器，先卸下武器
	if (UsingWeapon) DestroyUsingWeapon();
	ASSERT(UsingWeapon == nullptr);
	//在武装位置生成武器
	switch (Instance->GetCategory())
	{
	case E_EquipmentCategory::MeleeWeapon:
		{
			RemoveMeleeWeapon();
			CurEquipmentRef.MeleeWeapon = SpawnAtArmedSocket<AStMeleeWeapon>(Instance, OwnerMesh);
			UsingWeapon = CurEquipmentRef.MeleeWeapon;
			return true;
		}
	case E_EquipmentCategory::RangeWeapon:
		{
			RemoveRangeWeapon();
			SheatheShield();
			CurEquipmentRef.RangeWeapon = SpawnAtArmedSocket<AStRangeWeapon>(Instance, OwnerMesh);
			UsingWeapon = CurEquipmentRef.RangeWeapon;
			return true;
		}
	default:break;
	}
	return false;
}

void UEquipmentComponent::DestroyUsingWeapon()
{
	auto UsingWeaponInst = Cast<UEquipmentInstance>(UsingWeapon->GetItemInstance());
	switch (UsingWeaponInst->GetCategory())
	{
	case E_EquipmentCategory::MeleeWeapon:
		{
			//销毁当前武装的近战武器
			auto MeleeWeaponInst = Cast<UEquipmentInstance>(CurEquipmentRef.MeleeWeapon->GetItemInstance());
			RemoveMeleeWeapon();
			//在装备位置重新生成近战武器
			CurEquipmentRef.MeleeWeapon = SpawnAtEquippedSocket<AStMeleeWeapon>(MeleeWeaponInst,OwnerMesh);
			break;
		}
	case E_EquipmentCategory::RangeWeapon:
		{
			auto RangeWeaponInst = Cast<UEquipmentInstance>(CurEquipmentRef.RangeWeapon->GetItemInstance());
			//销毁当前武装的远程武器
			RemoveRangeWeapon();
			//在装备位置重新生成远程武器
			CurEquipmentRef.RangeWeapon = SpawnAtEquippedSocket<AStRangeWeapon>(RangeWeaponInst,OwnerMesh);
			break;
		}
	default:break;
	}
}

void UEquipmentComponent::ClearBag()
{
	if (CurEquipmentRef.MeleeWeapon) RemoveMeleeWeapon();
	if (CurEquipmentRef.RangeWeapon) RemoveRangeWeapon();
	if (CurEquipmentRef.Shield) RemoveShield();
	if (CurEquipmentRef.Armor) RemoveArmor();
	EquippableItems.Reset();
	EquipmentName2Obj.Reset();
}


E_WeaponKind UEquipmentComponent::GetUsingWeaponKind()
{
	if (!UsingWeapon) return E_WeaponKind::None;
	if (UsingWeapon->IsA(AMeleeWeapon::StaticClass()))
		return E_WeaponKind::MeleeWeapon;
	else return E_WeaponKind::RangeWeapon;
}



























